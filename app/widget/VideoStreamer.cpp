#include "VideoStreamer.h"
#include <iostream>
#include <fstream>
#ifdef USE_FFMPEG
#undef AV_NOPTS_VALUE
#define AV_NOPTS_VALUE int64_t(0x8000000000000000)

VideoStreamer::VideoStreamer(
	const char *ai_fileName, 
	const char *ai_format, 
        unsigned int width,
        unsigned int height,
        int ai_bitRate,
	int ai_frameRate,
	int ai_gopSize,
	int ai_bFrames,
	int ai_bufferSize)
	: m_formatContext(NULL) 
	, m_vStream(NULL)
	, m_vOutBuf(NULL)
	, m_vOutBufSize(ai_bufferSize)
	, m_stoprequested(false)
{
	m_frame[0] = NULL;
	m_frame[1] = NULL;

	// Initialize libavcodec, and register all codecs and formats
	avcodec_init();
	avcodec_register_all();
	av_register_all();

	// Allocate and initialize format context.
	//m_formatContext = CreateFormatContext(ai_fileName, NULL, NULL, ai_format);
	m_formatContext = CreateFormatContext(ai_fileName, ai_format, NULL, NULL);
	if (!m_formatContext)
		return;

	// Add video stream
	if (m_formatContext->oformat->video_codec != CODEC_ID_NONE)
		m_vStream = CreateVideoStream(
						m_formatContext,
                                                width,
                                                height,
                                                ai_bitRate,
						ai_frameRate, 
						ai_gopSize,
						ai_bFrames,
						PIX_FMT_YUV420P);

	// Set the output parameters (must be done even if no parameters) 
	av_set_parameters(m_formatContext, NULL );

	// Write out the format to the console
	dump_format(m_formatContext, 0, ai_fileName, 1);

	// Save sdp info to a file
	std::fstream sdpFile;
	sdpFile.open ("videostreamer.sdp", std::fstream::out | std::fstream::binary);
	char sdp[2048];
	avf_sdp_create(&m_formatContext, 1, sdp, 2048);
	sdpFile << sdp;
	sdpFile.close();
}

VideoStreamer::~VideoStreamer(void)
{
	// Release the streams
	if (m_formatContext)
	{
		for(unsigned int i = 0; i < m_formatContext->nb_streams; i++) 
		{
			av_freep(&m_formatContext->streams[i]->codec);
			av_freep(&m_formatContext->streams[i]);
		}

		// Release the format context
		av_free(m_formatContext);
	}
}

// Open the output file
int VideoStreamer::OpenVideo(void)
{
	// Find the video encoder
	AVCodecContext *c = m_vStream->codec;
	AVCodec *codec = avcodec_find_encoder(c->codec_id);
	if (!codec) 
	{
		std::cerr << "Error # VideoStreamer::OpenVideo: Codec not found" << std::endl;
		return -1;
	}

	// Open the codec
	if (avcodec_open(c, codec) < 0) 
	{
		std::cerr << "Error # VideoStreamer::OpenVideo: Could not open codec" << std::endl;
		return -2;
	}

	// Allocate the encoded raw picture.
	if (c->pix_fmt != PIX_FMT_BGR24)
		m_frame[0] = CreateFrame(c->pix_fmt, c->width, c->height);
	else
		m_frame[0] = avcodec_alloc_frame();
	m_frame[1] = CreateFrame(c->pix_fmt, c->width, c->height);

	if (!m_frame[0] || !m_frame[1])
	{
		std::cerr << "Error # VideoStreamer::OpenVideo: Could not allocate picture" << std::endl;
		return -3;
	}

	// Allocate output buffer for encoded frames if needed
	if (!(m_formatContext->oformat->flags & AVFMT_RAWPICTURE))
		m_vOutBuf = (uint8_t*)av_malloc(m_vOutBufSize);
	if (!m_vOutBuf)
		std::cerr << "Error # VideoStreamer::OpenVideo: Can't allocate output buffer" << std::endl;

	// Open the output file, if needed
	if (!(m_formatContext->oformat->flags & AVFMT_NOFILE) && 
		 (url_fopen(&m_formatContext->pb, m_formatContext->filename, URL_WRONLY) < 0)) 
	{
		std::cerr << "Error # VideoStreamer::OpenVideo: Could not open '" << m_formatContext->filename << "'" << std::endl;
		return -4;
	}

	// Start the thread that will write the frames
//	m_thread = boost::shared_ptr<boost::thread>(
//		new boost::thread(boost::bind(&VideoStreamer::Run, this)));

	return 0;
}

// Update the frame to stream
bool VideoStreamer::Update(ImgData* ai_image)
{
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock_(m_updateMutex);
//	if (!ValidateImage(ai_image))
//		return false;

	// If codec pixel format is diferent from PIX_FMT_BGR24 convert image data
	AVCodecContext *c = m_vStream->codec;
	if (c->pix_fmt == PIX_FMT_BGR24)
	{
		avpicture_fill(
			(AVPicture *)m_frame, 
                        (uint8_t *)ai_image->data,
			PIX_FMT_BGR24, 
			ai_image->width, ai_image->height);
	}
	else
	{
		if (m_frame[0]->data == NULL)
			return false;

		// As we have a BGR24 picture, we must convert it to the codec pixel format if needed
		struct SwsContext *convContext = sws_getContext(
				ai_image->width, ai_image->height,
				PIX_FMT_BGR24,
				c->width, c->height,
				c->pix_fmt,
				SWS_BICUBIC, 
				NULL, NULL, NULL);

		if (convContext == NULL) 
		{
			std::cout << "Cannot initialize the conversion context" << std::endl;
			return false;
		}

                int linesize[4] = {ai_image->width*4, 0, 0, 0};
		sws_scale(
			convContext,
                        (uint8_t **)&ai_image->data,
			linesize, 0, ai_image->height,
			m_frame[0]->data, m_frame[0]->linesize);

		sws_freeContext(convContext);
	}

        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
	AVFrame *tmp;
//	CV_SWAP(m_frame[0], m_frame[1], tmp);

	//m_frame[1] is now updated and will be used by the thread working method VideoStreamer::Run()
        m_firstFrame.signal();
	return true;
}


int VideoStreamer::Write(AVFrame *ai_picture)
{
	AVPacket packet;
	av_init_packet(&packet);
	packet.stream_index = m_vStream->index;

	if (m_formatContext->oformat->flags & AVFMT_RAWPICTURE) 
	{
		// Raw video case. The API will change slightly in the near	future for that
		packet.size = sizeof(AVPicture);
		packet.data = (uint8_t *)ai_picture;
		packet.flags |= PKT_FLAG_KEY;
	} 
	else 
	{
		packet.size = avcodec_encode_video(m_vStream->codec, m_vOutBuf, m_vOutBufSize, ai_picture);
		if (packet.size > 0) // if zero size, it means the image was buffered
		{
			packet.data = m_vOutBuf;

			AVFrame *codedFrame = m_vStream->codec->coded_frame;

			// pts: Presentation time stamp in time_base units.
			if (codedFrame->pts != AV_NOPTS_VALUE)
				packet.pts = av_rescale_q(codedFrame->pts, m_vStream->codec->time_base, m_vStream->time_base);

			if (codedFrame->key_frame)
				packet.flags |= PKT_FLAG_KEY;
		}
		else return 0;
	}

	return av_interleaved_write_frame(m_formatContext, &packet);
}



// Thread working method that writes out the AVFrames
void VideoStreamer::Run(void)
{
       // OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
	AVRational &tb = m_vStream->codec->time_base;
	double cycle = 1e3 * tb.num / (double)tb.den;

	// Wait for the first frame update and send the format context
        m_firstFrame.wait(&m_mutex,100000000);
	av_write_header(m_formatContext);
        m_mutex.unlock();

        osg::Timer_t _startTick =  osg::Timer::instance()->tick();
	uint64_t framesSent = 0;

	while(!m_stoprequested)
	{
                uint64_t elapsedTime = osg::Timer::instance()->delta_s(_startTick, osg::Timer::instance()->tick());
		uint64_t targetFramesSent = elapsedTime / cycle;

		if (framesSent < targetFramesSent) 
		{
                        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_mutex);
			Write(m_frame[1]);
			framesSent++;
		}
		else
		{
			sleep(1);
		}
	}

	// write the trailer, if any
	av_write_trailer(m_formatContext);
}

// Close the output video
void VideoStreamer::CloseVideo(void)
{
	m_stoprequested = true;
//	if (m_thread)
                m_thread.join();

	if (m_vStream->codec->codec)
		avcodec_close(m_vStream->codec);
	av_free(m_vOutBuf);

	// Close the output file
	if (m_formatContext->pb && !(m_formatContext->oformat->flags & AVFMT_NOFILE))
        url_fclose(m_formatContext->pb); 

	ReleaseFrame(m_frame[0], true);
	ReleaseFrame(m_frame[1], true);
}

// Allocate and initialize format context
AVFormatContext* CreateFormatContext(
        const char *ai_fileName,
        const char *ai_shortName,
        const char *ai_fileExtension,
        const char *ai_mimeType)
{
        AVFormatContext* formatContext = av_alloc_format_context();
        if (!formatContext)
        {
                std::cerr << "ERROR: Could not allocate format context at ::CreateFormatContext()." << std::endl;
                return NULL;
        }

        formatContext->oformat = guess_stream_format(ai_shortName, ai_fileExtension, ai_mimeType);
        if (!formatContext->oformat)
        {
                std::cerr << std::endl << "WARNING: Could not find suitable output format, using avi-mpeg4" << std::endl << std::endl;
                formatContext->oformat = guess_format("avi", NULL, NULL);
        }
        if (!formatContext->oformat)
        {
                std::cerr << "ERROR: Could not find suitable output format." << std::endl;
        }

        sprintf(formatContext->filename, "%s", ai_fileName);

        return formatContext;
}

// Create a new video stream
AVStream *CreateVideoStream(
        AVFormatContext* ai_formatContext,
        unsigned int width,
        unsigned int height,
        int ai_bitRate,
        int ai_frameRate,
        int ai_gopSize,
        int ai_bFrames,
        const PixelFormat& ai_pixFmt)
{
        // Create the new video stream
        AVStream *st = av_new_stream(ai_formatContext, 0);
        if (!st)
        {
                std::cerr << "ERROR: Could not allocate the video stream at ::CreateVideoStream()." << std::endl;
                return NULL;
        }

        // Initialize the codec context
        AVCodecContext *c = st->codec;
        c->codec_id		  = ai_formatContext->oformat->video_codec;
        c->codec_type	  = CODEC_TYPE_VIDEO;
        c->bit_rate		  = ai_bitRate;
        c->width		  = width;
        c->height		  = height;
        // time base: this is the fundamental unit of time (in seconds) in terms
        // of which frame timestamps are represented. for fixed-fps content,
        // timebase should be 1/framerate and timestamp increments should be
        // identically 1.
        c->time_base.num  = 1;
        c->time_base.den  = ai_frameRate;
        c->gop_size		  = ai_gopSize;
        c->pix_fmt		  = ai_pixFmt;

        /*
        if (c->codec_id == CODEC_ID_MPEG2VIDEO)
                c->max_b_frames = ai_bFrames; // we also add B frames
        if (c->codec_id == CODEC_ID_MPEG1VIDEO)
        {
                // needed to avoid using macroblocks in which some coeffs overflow
                // this doesnt happen with normal video, it just happens here as the
                // motion of the chroma plane doesnt match the luma plane
                c->mb_decision=2;
        }

        // Some formats want stream headers to be seperate
        const char *fn = ai_formatCtx->oformat->name;
        if (!strcmp(fn, "mp4") || !strcmp(fn, "mov") || !strcmp(fn, "3gp"))
                c->flags |= CODEC_FLAG_GLOBAL_HEADER;
        */

        return st;
}

// Allocate memory for a new frame
AVFrame *CreateFrame(int ai_pixFmt, int ai_width, int ai_height)
{
        // Alloc frame header
        AVFrame *frame = avcodec_alloc_frame();
        if (!frame)
                return NULL;

        // Alloc frame data
        uint8_t *data = (uint8_t*)av_malloc(avpicture_get_size((PixelFormat)ai_pixFmt, ai_width, ai_height));
        if (!data)
        {
                av_freep(frame);
                return NULL;
        }

        avpicture_fill((AVPicture *)frame, data, (PixelFormat)ai_pixFmt, ai_width, ai_height);

        return frame;
}

// Release memory from a AVFrame
void ReleaseFrame(AVFrame *ai_frame, bool ai_releaseData)
{
        if (ai_frame)
        {
                if (ai_releaseData && ai_frame->data[0])
                        av_freep(ai_frame->data[0]);

                av_freep(ai_frame);
        }
}

/*bool ValidateImage(IplImage *ai_image)
{
        if ((ai_image == NULL) ||
                (ai_image->nChannels != 3) ||
                (ai_image->depth != IPL_DEPTH_8U) ||
                (ai_image->width%2 != 0) ||
                (ai_image->height%2 != 0) ||
                (ai_image->imageSize != avpicture_get_size(PIX_FMT_BGR24, ai_image->width, ai_image->height)))
        {
                std::cout << "ERROR: Attempt to stream an invalid IplImage detected at ::ValidateImage()." << std::endl;
                return false;
        }

        return true;
}*/
#endif
