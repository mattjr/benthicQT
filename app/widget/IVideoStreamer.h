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

Interfaz de Video Streamer

\*************************************************************************/

#ifndef __IVIDEO_STREAMER_H__
#define __IVIDEO_STREAMER_H__

struct ImgData{
unsigned int width;
unsigned int height;
unsigned char *data;
};

struct IVideoStreamer
{
	virtual ~IVideoStreamer() {}
        IVideoStreamer() :_isOpen(false){}
	virtual int  OpenVideo(void) = 0;
        virtual bool Update(ImgData* ai_image) = 0;
	virtual void CloseVideo(void) = 0;
        virtual void Run(void) = 0;
        virtual bool IsOpen(void) {return _isOpen;};

        static const bool enabled;
        bool _isOpen;
};
#ifndef USE_FFMPEG

typedef class NullVideoStreamer: public IVideoStreamer{
    virtual int  OpenVideo(void) {}
    virtual bool Update(ImgData* ai_image) {}
    virtual void CloseVideo(void) {}
    virtual void Run(void) {}
    virtual bool IsOpen(void) {return false;}
    static const bool enabled = false;
}OSGVideoStreamer;

#else
class VideoStreamer;
typedef  VideoStreamer OSGVideoStreamer;
#endif

#endif // __IVIDEO_STREAMER_H__
