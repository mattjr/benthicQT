#ifndef OSGMOVIECALLBACK_H
#define OSGMOVIECALLBACK_H

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <iostream>
#include <sstream>
#include <string.h>
#include "IVideoStreamer.h"

class WindowCaptureCallback : public osg::Camera::DrawCallback
{
    public:

        enum Mode
        {
            READ_PIXELS,
            SINGLE_PBO,
            DOUBLE_PBO,
            TRIPLE_PBO
        };

        enum FramePosition
        {
            START_FRAME,
            END_FRAME
        };

        struct ContextData : public osg::Referenced
        {

            ContextData(osg::GraphicsContext* gc, Mode mode, GLenum readBuffer, const std::string& name):
                _gc(gc),
                _mode(mode),
                _readBuffer(readBuffer),
                _fileName(name),
                _pixelFormat(GL_BGRA),
                _type(GL_UNSIGNED_BYTE),
                _width(0),
                _height(0),
                _currentImageIndex(0),
                _currentPboIndex(0),
                _reportTimingFrequency(0),
                _numTimeValuesRecorded(0),
                _timeForReadPixels(0.0),
                _timeForFullCopy(0.0),
                _timeForMemCpy(0.0),
                _recording(false)


            {
                _previousFrameTick = osg::Timer::instance()->tick();

                if (gc->getTraits())
                {
                    if (gc->getTraits()->alpha)
                    {
                        osg::notify(osg::NOTICE)<<"Select GL_BGRA read back format"<<std::endl;
                        _pixelFormat = GL_BGRA;
                    }
                    else
                    {
                        osg::notify(osg::NOTICE)<<"Select GL_BGR read back format"<<std::endl;
                        _pixelFormat = GL_BGR;
                    }
                }

                getSize(gc, _width, _height);

                std::cout<<"Window size "<<_width<<", "<<_height<<std::endl;

                // single buffered image
                _imageBuffer.push_back(new osg::Image);

                // double buffer PBO.
                switch(_mode)
                {
                    case(READ_PIXELS):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with out PixelBufferObject."<<std::endl;
                        break;
                    case(SINGLE_PBO):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a single PixelBufferObject."<<std::endl;
                        _pboBuffer.push_back(0);
                        break;
                    case(DOUBLE_PBO):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a double buffer PixelBufferObject."<<std::endl;
                        _pboBuffer.push_back(0);
                        _pboBuffer.push_back(0);
                        break;
                    case(TRIPLE_PBO):
                        osg::notify(osg::NOTICE)<<"Reading window usig glReadPixels, with a triple buffer PixelBufferObject."<<std::endl;
                        _pboBuffer.push_back(0);
                        _pboBuffer.push_back(0);
                        _pboBuffer.push_back(0);
                        break;
                    default:
                        break;
                }
            }

            void getSize(osg::GraphicsContext* gc, int& width, int& height)
            {
                if (gc->getTraits())
                {
                    width = gc->getTraits()->width;
                    height = gc->getTraits()->height;
                }
            }

            void updateTimings(osg::Timer_t tick_start,
                               osg::Timer_t tick_afterReadPixels,
                               osg::Timer_t tick_afterMemCpy,
                               unsigned int dataSize);

            void read()
            {
                osg::BufferObject::Extensions* ext = osg::BufferObject::getExtensions(_gc->getState()->getContextID(),true);

                if (ext->isPBOSupported() && !_pboBuffer.empty())
                {
                    if (_pboBuffer.size()==1)
                    {
                        singlePBO(ext);
                    }
                    else
                    {
                        multiPBO(ext);
                    }
                }
                else
                {
                    readPixels();
                }
            }

            void readPixels();
            

            void singlePBO(osg::BufferObject::Extensions* ext);

            void multiPBO(osg::BufferObject::Extensions* ext);

            typedef std::vector< osg::ref_ptr<osg::Image> >             ImageBuffer;
            typedef std::vector< GLuint > PBOBuffer;

            osg::GraphicsContext*   _gc;
            Mode                    _mode;
            GLenum                  _readBuffer;
            std::string             _fileName;

            GLenum                  _pixelFormat;
            GLenum                  _type;
            int                     _width;
            int                     _height;

            unsigned int            _currentImageIndex;
            ImageBuffer             _imageBuffer;

            unsigned int            _currentPboIndex;
            PBOBuffer               _pboBuffer;

            unsigned int            _reportTimingFrequency;
            unsigned int            _numTimeValuesRecorded;
            double                  _timeForReadPixels;
            double                  _timeForFullCopy;
            double                  _timeForMemCpy;
            osg::Timer_t            _previousFrameTick;
            OSGVideoStreamer        _videoStreamer;
            bool _recording;
        };

        WindowCaptureCallback(Mode mode=READ_PIXELS, FramePosition position=START_FRAME, GLenum readBuffer=GL_BACK):
            _mode(mode),
            _position(position),
            _readBuffer(readBuffer),
            _recording(false)
        {
        }

        FramePosition getFramePosition() const { return _position; }

        ContextData* createContextData(osg::GraphicsContext* gc) const
        {
            std::stringstream filename;
            filename << "test_"<<_contextDataMap.size()<<".jpg";
            return new ContextData(gc, _mode, _readBuffer, filename.str());
        }

        ContextData* getContextData(osg::GraphicsContext* gc) const
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
            osg::ref_ptr<ContextData>& data = _contextDataMap[gc];
            if (!data) data = createContextData(gc);

            return data.get();
        }

        virtual void operator () (osg::RenderInfo& renderInfo) const
        {
            if(!_recording)
                return;

            glReadBuffer(_readBuffer);

            osg::GraphicsContext* gc = renderInfo.getState()->getGraphicsContext();
            osg::ref_ptr<ContextData> cd = getContextData(gc);
            cd->read();
        }
         void startRecording(osg::GraphicsContext* gc){
             if(_recording){
                 std::cerr << "Already Recording\n";
                 return;
             }
             osg::ref_ptr<ContextData> cd = getContextData(gc);
             cd->_videoStreamer.OpenVideo();
             _recording=true;
         }

         void stopRecording(osg::GraphicsContext* gc){
             if(!_recording){
                 std::cerr << "Not Recording\n";
                 return;
             }
             osg::ref_ptr<ContextData> cd = getContextData(gc);
             cd->_videoStreamer.CloseVideo();
             _recording=false;
         }


        typedef std::map<osg::GraphicsContext*, osg::ref_ptr<ContextData> > ContextDataMap;

        Mode                        _mode;
        FramePosition               _position;
        GLenum                      _readBuffer;
        mutable OpenThreads::Mutex  _mutex;
        mutable ContextDataMap      _contextDataMap;
        bool _recording;

};

void addMovieCallbackToViewer(osgViewer::ViewerBase& viewer, WindowCaptureCallback* callback);

#endif // OSGMOVIECALLBACK_H
