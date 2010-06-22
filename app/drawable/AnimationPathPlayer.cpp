#include "AnimationPathPlayer.hpp"

#include <osgDB/fstream>

using namespace osgGA;
void MyAnimationPath::insert(double time,const ControlPoint& controlPoint)
{
    _timeControlPointMap[time] = controlPoint;
}

bool MyAnimationPath::getInterpolatedControlPoint(double time,ControlPoint& controlPoint) const
{
    if (_timeControlPointMap.empty()) return false;

    switch(_loopMode)
    {
        case(SWING):
        {
            double modulated_time = (time - getFirstTime())/(getPeriod()*2.0);
            double fraction_part = modulated_time - floor(modulated_time);
            if (fraction_part>0.5) fraction_part = 1.0-fraction_part;

            time = getFirstTime()+(fraction_part*2.0) * getPeriod();
            break;
        }
        case(LOOP):
        {
            double modulated_time = (time - getFirstTime())/getPeriod();
            double fraction_part = modulated_time - floor(modulated_time);
            time = getFirstTime()+fraction_part * getPeriod();
            break;
        }
        case(NO_LOOPING):
            // no need to modulate the time.
            break;
    }



    TimeControlPointMap::const_iterator second = _timeControlPointMap.lower_bound(time);
    if (second==_timeControlPointMap.begin())
    {
        controlPoint = second->second;
    }
    else if (second!=_timeControlPointMap.end())
    {
        TimeControlPointMap::const_iterator first = second;
        --first;

        // we have both a lower bound and the next item.

        // delta_time = second.time - first.time
        double delta_time = second->first - first->first;

        if (delta_time==0.0)
            controlPoint = first->second;
        else
        {
            controlPoint.interpolate((time - first->first)/delta_time,
                            first->second,
                            second->second);
        }
    }
    else // (second==_timeControlPointMap.end())
    {
        controlPoint = _timeControlPointMap.rbegin()->second;
    }
    return true;
}

AnimationPathPlayer::AnimationPathPlayer(MyAnimationPath* animationPath)
{
    _printOutTimingInfo = true;

    _animationPath = animationPath;
    _timeOffset = 0.0;
    _timeScale = 1.0;
    _isPaused = false;

    _realStartOfTimedPeriod = 0.0;
    _animStartOfTimedPeriod = 0.0;
    _numOfFramesSinceStartOfTimedPeriod = -1; // need to init.
}



void AnimationPathPlayer::home(double currentTime)
{
    if (_animationPath.valid())
    {
        _timeOffset = _animationPath->getFirstTime()-currentTime;

    }
    // reset the timing of the animation.
    _numOfFramesSinceStartOfTimedPeriod=-1;
}

void AnimationPathPlayer::home(const GUIEventAdapter& ea,GUIActionAdapter&)
{
    home(ea.getTime());
}

void AnimationPathPlayer::init(const GUIEventAdapter& ea,GUIActionAdapter& aa)
{
    home(ea,aa);
}

bool AnimationPathPlayer::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us)
{
    if( !valid() ) return false;

    switch( ea.getEventType() )
    {
    case GUIEventAdapter::FRAME:
        if( _isPaused )
        {
            handleFrame( _pauseTime );
        }
        else
        {
            handleFrame( ea.getTime() );
        }
        return false;
    case GUIEventAdapter::KEYDOWN:
            if (ea.getKey()==' ')
            {
                _isPaused = false;
                _timeScale = 1.0;

                home(ea,us);
                us.requestRedraw();
                us.requestContinuousUpdate(false);

                return true;
            }
            else if (ea.getKey()==')')
            {
                double time = _isPaused ? _pauseTime : ea.getTime();
                double animationTime = (time+_timeOffset)*_timeScale;

                _timeScale *= 1.1;

                OSG_NOTICE<<"Animation speed = "<<_timeScale*100<<"%"<<std::endl;

                // adjust timeOffset so the current animationTime does change.
                _timeOffset = animationTime/_timeScale - time;

                return true;
            }
            else if (ea.getKey()=='(')
            {
                double time = _isPaused ? _pauseTime : ea.getTime();
                double animationTime = (time+_timeOffset)*_timeScale;

                _timeScale /= 1.1;

                OSG_NOTICE<<"Animation speed = "<<_timeScale*100<<"%"<<std::endl;

                // adjust timeOffset so the current animationTime does change.
                _timeOffset = animationTime/_timeScale - time;

                return true;
            }
            else if(ea.getKey() == 'p')
            {
                if( _isPaused )
                {
                    _isPaused = false;
                    _timeOffset -= ea.getTime() - _pauseTime;
                }
                else
                {
                    _isPaused = true;
                    _pauseTime = ea.getTime();
                }
                us.requestRedraw();
                us.requestContinuousUpdate(false);
                return true;
            }

        break;
        default:
            break;
    }
    return false;
}

void AnimationPathPlayer::getUsage(osg::ApplicationUsage& usage) const
{
    usage.addKeyboardMouseBinding("AnimationPath: Space","Reset the viewing position to start of animation");
    usage.addKeyboardMouseBinding("AnimationPath: p","Pause/resume animation.");
    usage.addKeyboardMouseBinding("AnimationPath: <","Slow down animation speed.");
    usage.addKeyboardMouseBinding("AnimationPath: <","Speed up animation speed.");
}

void AnimationPathPlayer::handleFrame( double time )
{
    MyAnimationPath::ControlPoint cp;

    double animTime = (time+_timeOffset)*_timeScale;
    _animationPath->getInterpolatedControlPoint( animTime, cp );

    if (_numOfFramesSinceStartOfTimedPeriod==-1)
    {
        _realStartOfTimedPeriod = time;
        _animStartOfTimedPeriod = animTime;

    }

    ++_numOfFramesSinceStartOfTimedPeriod;

    if (_printOutTimingInfo)
    {
        double animDelta = (animTime-_animStartOfTimedPeriod);
        if (animDelta>=_animationPath->getPeriod())
        {

            double delta = time-_realStartOfTimedPeriod;

            double frameRate = (double)_numOfFramesSinceStartOfTimedPeriod/delta;
            OSG_NOTICE <<"AnimatonPath completed in "<<delta<<" seconds, completing "<<_numOfFramesSinceStartOfTimedPeriod<<" frames,"<<std::endl;
            OSG_NOTICE <<"             average frame rate = "<<frameRate<<std::endl;

            // reset counters for next loop.
            _realStartOfTimedPeriod = time;
            _animStartOfTimedPeriod = animTime;

            _numOfFramesSinceStartOfTimedPeriod = 0;
        }
    }

    cp.getMatrix( _matrix );
}
