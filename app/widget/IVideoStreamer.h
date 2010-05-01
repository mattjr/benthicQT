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

	virtual int  OpenVideo(void) = 0;
        virtual bool Update(ImgData* ai_image) = 0;
	virtual void CloseVideo(void) = 0;
};

#endif // __IVIDEO_STREAMER_H__
