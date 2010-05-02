/*************************************************************************\

Librería de Sensor Visual (LibSV)

http://code.google.com/p/pfcfnm/

© 2007, Fernando N. M.         
This project is released under the GPL license. 
See http://www.gnu.org/licenses/gpl.html for the full text.

$HeadURL:  $
$Revision:  $
$Author:  $
$Date:  $

Declaración de la clase de video streaming. Esta clase es un wrapper
de la funcionalidad de emisión de video de la librería FFMPEG, que
permite componer dicho flujo de video a partir de las imágenes en
formato Ipl, propias de OpenCV.

Los binarios de ffmpeg pueden obtenerse en:
http://arrozcru.no-ip.org/ffmpeg_builds/
http://code.google.com/p/msinttypes/
http://www.codeproject.com/KB/cpp/Using_FFMpeg.aspx

En la siguiente dirección se puede encontrar una versión de highgui
mejorada y capaz de emitir rtp mediante FFMPEG.
http://code.google.com/p/eyepatch/

\*************************************************************************/

#ifndef __VIDEO_STREAMER_H__
#define __VIDEO_STREAMER_H__

#include <string>
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#include <OpenThreads/Condition>
#include <OpenThreads/Thread>
#include <osg/Timer>

#include <stdint.h>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

enum PixelFormat;

AVFormatContext* CreateFormatContext(
        const char *ai_fileName,
        const char *ai_shortName,
        const char *ai_fileExtension,
        const char *ai_mimeType);

AVStream* CreateVideoStream(
        AVFormatContext* ai_formatContext,
        unsigned int width,
        unsigned int height,
        int ai_bitRate,
        int ai_frameRate,
        int ai_gopSize,
        int ai_bFrames,
        const PixelFormat& ai_pixFmt);

AVFrame* CreateFrame(int ai_pixFmt, int ai_width, int ai_height);

void ReleaseFrame(AVFrame* ai_frame, bool ai_releaseData);

//bool ValidateImage(IplImage *ai_image);

struct AVStream;
struct AVFormatContext;
struct AVFrame;
enum PixelFormat;
typedef uint8_t unsigned __int8   ;
class VideoStreamer;
class WorkThread : public OpenThreads::Thread
{
        public:
    WorkThread(IVideoStreamer *streamObj):_streamer(streamObj){}
    virtual void run(){_streamer->Run();};
private:
    IVideoStreamer *_streamer;
};

class VideoStreamer : IVideoStreamer
{
public:
	VideoStreamer(
		const char *ai_fileName = "out.avi", 
		const char *ai_format = "avi",
                unsigned int width=640,
                unsigned int height=480, // size must be a multiple of two (because of fourcc)
		int ai_bitRate = 500000, 
		int ai_frameRate = 25,
		int ai_gopSize = 12,	// emit one I-frame every "ai_gopSize" frames at most
		int ai_bFrames = 2,     // emit this number of B-Frames in each gop
		int ai_bufferSize = 2e5);
	virtual ~VideoStreamer();

	virtual int OpenVideo(void);
        virtual bool Update(ImgData* ai_image);
	virtual void CloseVideo(void);
        static const bool enabled = true;

private:


	int Write(AVFrame *ai_picture);
	void Run(void);

	// Video format context and stream
	AVFormatContext *m_formatContext;
	AVStream *m_vStream;

	// Video buffers
	AVFrame *m_frame[2];
	uint8_t *m_vOutBuf;
	int m_vOutBufSize;

	// Threading variables
	bool m_stoprequested;
        WorkThread m_thread;
        OpenThreads::Mutex m_mutex;
        OpenThreads::Condition m_firstFrame;
        OpenThreads::Mutex m_updateMutex; // to avoid reentrancy. Check if it is worth...
        friend class WorkThread;
};


#endif // __VIDEO_STREAMER_H__
