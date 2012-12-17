/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#ifndef ANIM_PATH_PLAYER
#define ANIM_PATH_PLAYER 1
#include <osg/Notify>
#include <osg/Version>
#if (OPENSCENEGRAPH_MAJOR_VERSION>2) || (OPENSCENEGRAPH_MAJOR_VERSION==2&&OPENSCENEGRAPH_MINOR_VERSION>=9 && OPENSCENEGRAPH_PATCH_VERSION >8)
#define MANIP_INHERIT osgGA::CameraManipulator
#include <osgGA/CameraManipulator>
#else
#define MANIP_INHERIT osgGA::MatrixManipulator
#include <osgGA/MatrixManipulator>
#endif

class  MyAnimationPath : public virtual osg::Object
{
    public:

        MyAnimationPath():_loopMode(NO_LOOPING) {}

        MyAnimationPath(const MyAnimationPath& ap, const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY):
            Object(ap,copyop),
            _timeControlPointMap(ap._timeControlPointMap),
            _loopMode(ap._loopMode) {}

       META_Object(osg,MyAnimationPath);

        class ControlPoint
        {
        public:
            ControlPoint()
               {}

            ControlPoint(const osg::Vec3d& center):
                _center(center),
                _orientation(),
                _distance(0.0),
                _tilt(0.0){}

            ControlPoint(const osg::Vec3d& center, const osg::Quat& orientation):
                _center(center),
                _orientation(orientation),
                _distance(0.0),
                _tilt(0.0) {}

            ControlPoint(const osg::Vec3d& center, const osg::Quat& orientation, const double distance, const double tilt):
                _center(center),
                _orientation(orientation),
                _distance(distance),
                _tilt(tilt)
                {}

            void setCenter(const osg::Vec3d& center) { _center = center; }
            const osg::Vec3d& getCenter() const { return _center; }

            void setOrientation(const osg::Quat& orientation) { _orientation = orientation; }
            const osg::Quat& getOrientation() const { return _orientation; }

            void setDistance(const double &distance) { _distance = distance; }
            const double& getDistance() const { return _distance; }

            void setTilt(const double &tilt) { _tilt = tilt; }
            const double& getTilt() const { return _tilt; }

            inline void interpolate(float ratio,const ControlPoint& first, const ControlPoint& second)
            {
                float one_minus_ratio = 1.0f-ratio;
                _center = first._center*one_minus_ratio + second._center*ratio;
                _orientation.slerp(ratio,first._orientation,second._orientation);
                _distance = first._distance*one_minus_ratio + second._distance*ratio;
                _tilt = first._tilt*one_minus_ratio + second._tilt*ratio;
            }

            inline void interpolate(double ratio,const ControlPoint& first, const ControlPoint& second)
            {
                double one_minus_ratio = 1.0f-ratio;
                _center = first._center*one_minus_ratio + second._center*ratio;
                _orientation.slerp(ratio,first._orientation,second._orientation);
                _distance = first._distance*one_minus_ratio + second._distance*ratio;
                _tilt = first._tilt*one_minus_ratio + second._tilt*ratio;
            }

            inline void getMatrix(osg::Matrixf& matrix) const
            {
                matrix=osg::Matrixd::translate(0.0,0.0,_distance)* osg::Matrixd::rotate(osg::Quat(osg::DegreesToRadians(_tilt),osg::Vec3d(1,0,0)))*
                          osg::Matrixd::rotate(_orientation)*osg::Matrixd::translate(_center);

            }

            inline void getMatrix(osg::Matrixd& matrix) const
            {
                matrix=osg::Matrixd::translate(0.0,0.0,_distance)* osg::Matrixd::rotate(osg::Quat(osg::DegreesToRadians(_tilt),osg::Vec3d(1,0,0)))*
                          osg::Matrixd::rotate(_orientation)*osg::Matrixd::translate(_center);
            }

            inline void getInverse(osg::Matrixf& matrix) const
            {
                osg::Matrixf m;
                m=osg::Matrixd::translate(0.0,0.0,_distance)* osg::Matrixd::rotate(osg::Quat(osg::DegreesToRadians(_tilt),osg::Vec3d(1,0,0)))*
                          osg::Matrixd::rotate(_orientation)*osg::Matrixd::translate(_center);

                matrix.inverse(m);
            }

            inline void getInverse(osg::Matrixd& matrix) const
            {
                osg::Matrixd m;
                m=osg::Matrixd::translate(0.0,0.0,_distance)* osg::Matrixd::rotate(osg::Quat(osg::DegreesToRadians(_tilt),osg::Vec3d(1,0,0)))*
                          osg::Matrixd::rotate(_orientation)*osg::Matrixd::translate(_center);

                matrix.inverse(m);
            }

        protected:

            osg::Vec3d _center;
            osg::Quat _orientation;
            double _distance;
            double _tilt;


        };


        /** Given a specific time, return the transformation matrix for a point. */
        bool getMatrix(double time,osg::Matrixf& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getMatrix(matrix);
            return true;
        }

        /** Given a specific time, return the transformation matrix for a point. */
        bool getMatrix(double time,osg::Matrixd& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getMatrix(matrix);
            return true;
        }

        /** Given a specific time, return the inverse transformation matrix for a point. */
        bool getInverse(double time,osg::Matrixf& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getInverse(matrix);
            return true;
        }

        bool getInverse(double time,osg::Matrixd& matrix) const
        {
            ControlPoint cp;
            if (!getInterpolatedControlPoint(time,cp)) return false;
            cp.getInverse(matrix);
            return true;
        }

        /** Given a specific time, return the local ControlPoint frame for a point. */
        virtual bool getInterpolatedControlPoint(double time,ControlPoint& controlPoint) const;

        /** Insert a control point into the MyAnimationPath.*/
        void insert(double time,const ControlPoint& controlPoint);

        double getFirstTime() const { if (!_timeControlPointMap.empty()) return _timeControlPointMap.begin()->first; else return 0.0;}
        double getLastTime() const { if (!_timeControlPointMap.empty()) return _timeControlPointMap.rbegin()->first; else return 0.0;}
        double getPeriod() const { return getLastTime()-getFirstTime();}

        enum LoopMode
        {
            SWING,
            LOOP,
            NO_LOOPING
        };

        void setLoopMode(LoopMode lm) { _loopMode = lm; }

        LoopMode getLoopMode() const { return _loopMode; }


        typedef std::map<double,ControlPoint> TimeControlPointMap;

        void setTimeControlPointMap(TimeControlPointMap& tcpm) { _timeControlPointMap=tcpm; }

        TimeControlPointMap& getTimeControlPointMap() { return _timeControlPointMap; }

        const TimeControlPointMap& getTimeControlPointMap() const { return _timeControlPointMap; }

        bool empty() const { return _timeControlPointMap.empty(); }

        void clear() { _timeControlPointMap.clear(); }

        /** Read the animation path from a flat ASCII file stream. */
        void read(std::istream& in);

        /** Write the animation path to a flat ASCII file stream. */
        void write(std::ostream& out) const;

        /** Write the control point to a flat ASCII file stream. */
        void write(TimeControlPointMap::const_iterator itr, std::ostream& out) const;

    protected:

        virtual ~MyAnimationPath() {}

        TimeControlPointMap _timeControlPointMap;
        LoopMode            _loopMode;

};




//
// The AnimationPathPlayer is a Matrix Manipulator that reads an
// animation path from a file and plays it back.  The file is expected
// to be ascii and a succession of lines with 8 floating point values
// per line.  The succession of values are:
// time  px py pz ax ay az aw
// where:
//    time = elapsed time in seconds from the beginning of the animation
//    px py pz = World position in cartesian coordinates
//    ax ay az aw = Orientation (attitude) defined as a quaternion
class AnimationPathPlayer : public MANIP_INHERIT {

    public:

        AnimationPathPlayer( MyAnimationPath* animationPath=0 );


        virtual const char* className() const { return "AnimationPath"; }

        void setPrintOutTimingInfo(bool printOutTimingInfo) { _printOutTimingInfo=printOutTimingInfo; }
        bool getPrintOutTimingInfo() const { return _printOutTimingInfo; }

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByMatrix(const osg::Matrixd& matrix) { _matrix = matrix; }

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByInverseMatrix(const osg::Matrixd& matrix) { _matrix.invert(matrix); }

        /** get the position of the manipulator as 4x4 Matrix.*/
        virtual osg::Matrixd getMatrix() const { return _matrix; }

        /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
        virtual osg::Matrixd getInverseMatrix() const { return osg::Matrixd::inverse(_matrix); }


        void setAnimationPath( MyAnimationPath* animationPath ) { _animationPath=animationPath; }

        MyAnimationPath* getAnimationPath() { return _animationPath.get(); }

        const MyAnimationPath* getAnimationPath() const { return _animationPath.get(); }

        bool valid() const { return _animationPath.valid(); }

        void init(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

        void home(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
        void home(double currentTime);

        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

        /** Get the keyboard and mouse usage of this manipulator.*/
        virtual void getUsage(osg::ApplicationUsage& usage) const;

    protected:

        bool _valid;

        bool _printOutTimingInfo;

        void handleFrame( double time );

        osg::ref_ptr<MyAnimationPath> _animationPath;

        double  _timeOffset;
        double  _timeScale;
        double  _pauseTime;
        bool    _isPaused;

        double  _realStartOfTimedPeriod;
        double  _animStartOfTimedPeriod;
        int     _numOfFramesSinceStartOfTimedPeriod;

        osg::Matrixd _matrix;

};




#endif
