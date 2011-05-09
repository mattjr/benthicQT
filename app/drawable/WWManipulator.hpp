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

#ifndef WORLDWINDMANIPULATORNEW
#define WORLDWINDMANIPULATORNEW 1
#include <osg/Version>
#if (OPENSCENEGRAPH_MAJOR_VERSION>2) || (OPENSCENEGRAPH_MAJOR_VERSION==2&&OPENSCENEGRAPH_MINOR_VERSION>=9 && OPENSCENEGRAPH_PATCH_VERSION >8)
#define MANIP_INHERIT osgGA::CameraManipulator
#include <osgGA/CameraManipulator>
#else
#define MANIP_INHERIT osgGA::MatrixManipulator
#include <osgGA/MatrixManipulator>
#endif

#include <osg/Quat>
#include <osgViewer/Viewer>
#include <osgSim/HeightAboveTerrain>
#include <osgText/Text>
#include "AnimationPathPlayer.hpp"
using namespace osgGA;
using namespace osg;

class WorldWindManipulatorNew : public MANIP_INHERIT
{
public:
    WorldWindManipulatorNew(osgViewer::Viewer *viewer,osgText::Text *text, bool invertMouse,osg::Matrixd *mat,MyAnimationPath *animationPath);

    virtual const char* className() const { return "WorldWind"; }

    /** set the position of the matrix manipulator using a 4x4 Matrix.*/
    virtual void setByMatrix(const osg::Matrixd& matrix);

    /** set the position of the matrix manipulator using a 4x4 Matrix.*/
    virtual void setByInverseMatrix(const osg::Matrixd& matrix) { setByMatrix(osg::Matrixd::inverse(matrix)); }

    /** get the position of the manipulator as 4x4 Matrix.*/
    virtual osg::Matrixd getMatrix() const;

    /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
    virtual osg::Matrixd getInverseMatrix() const;

    /** Get the FusionDistanceMode. Used by SceneView for setting up setereo convergence.*/
    virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const { return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE; }

    /** Get the FusionDistanceValue. Used by SceneView for setting up setereo convergence.*/
    virtual float getFusionDistanceValue() const { return _distance; }

    /** Attach a node to the manipulator.
            Automatically detaches previously attached node.
            setNode(NULL) detaches previously nodes.
            Is ignored by manipulators which do not require a reference model.*/
    virtual void setNode(osg::Node*);

    /** Return node if attached.*/
    virtual const osg::Node* getNode() const;

    /** Return node if attached.*/
    virtual osg::Node* getNode();

    /** Move the camera to the default position.
            May be ignored by manipulators if home functionality is not appropriate.*/
    virtual void home(const GUIEventAdapter& ea,GUIActionAdapter& us);
    virtual void home(double);

    /** Start/restart the manipulator.*/
    virtual void init(const GUIEventAdapter& ea,GUIActionAdapter& us);

    /** handle events, return true if handled, false otherwise.*/
    virtual bool handle(const GUIEventAdapter& ea,GUIActionAdapter& us);
    void handleFrameAnim( double time );
    /** Get the keyboard and mouse usage of this manipulator.*/
    virtual void getUsage(osg::ApplicationUsage& usage) const;


    /** set the minimum distance (as ratio) the eye point can be zoomed in towards the
            center before the center is pushed forward.*/        
    void setMinimumZoomScale(double minimumZoomScale) { _minimumZoomScale=minimumZoomScale; }

    /** get the minimum distance (as ratio) the eye point can be zoomed in */
    double getMinimumZoomScale() const { return _minimumZoomScale; }

    /** Set the center of the trackball. */
    void setCenter(const osg::Vec3d& center) { _center = center; }

    /** Get the center of the trackball. */
    const osg::Vec3d& getCenter() const { return _center; }
    void moveTo(const osg::Vec3d& center, const osg::Quat& orientation, const double distance, const double tilt){
        _center=center;
        m_Orientation=orientation;
        _distance=distance;
        _tilt=tilt;

        _targetCenter=center;
        _targetOrientation=orientation;
        _targetDistance=distance;
        _targetTilt=tilt;

    }
    /** Set the rotation of the trackball. */
    void setRotation(const osg::Quat& rotation) { m_Orientation = rotation; }

    /** Get the rotation of the trackball. */
    const osg::Quat& getRotation() const { return m_Orientation; }

    /** Set the distance of the trackball. */
    void setDistance(double distance) { _distance = distance; }

    void setTargetDistance(double distance) { _targetDistance = distance; }

    void setTargetCenter(const osg::Vec3d& center) { _targetCenter=center; }
    void setIsAnim(bool isanim){_isAnimating=isanim;}
    void setTargetOrientation(const osg::Quat ori) { _targetOrientation=ori; }
    osg::Quat getTargetOrientation() { return _targetOrientation; }
    double getTargetTilt(){return _targetTilt;}
    void setTargetTilt(double tilt){_targetTilt=tilt;}

    double getTargetDistance() { return _targetDistance; }

    osg::Vec3d getTargetCenter() { return  _targetCenter; }
    bool notMoving(void);
    /** Get the distance of the trackball. */
    double getDistance() const { return _distance; }

    /** Set the size of the trackball. */
    void setWorldWindSize(float size);

    /** Get the size of the trackball. */
    float getWorldWindSize() const { return _trackballSize; }
    double getHomeDist(void){return _homeDist;}
    bool _isAnimating;
    void togglePaused();
    bool getPaused(){return _isPaused;}
protected:
    bool _doRecomp;
    virtual ~WorldWindManipulatorNew();
    /** Anim variables */
    osg::ref_ptr<MyAnimationPath> _animationPath;
    bool _printOutTimingInfo;

    double  _timeOffset;
    double  _timeScale;
    double  _pauseTime;
    bool    _isPaused;
    bool _pausedChanged;

    double  _realStartOfTimedPeriod;
    double  _animStartOfTimedPeriod;
    int     _numOfFramesSinceStartOfTimedPeriod;

    /** Reset the internal GUIEvent stack.*/
    void flushMouseEventStack();
    /** Add the current mouse GUIEvent to internal stack.*/
    void addMouseEvent(const GUIEventAdapter& ea);

    void RecomputeTerrainIntersection();
    void RotationHeading(float y);
    void computePosition(const osg::Vec3& eye,const osg::Vec3& lv,const osg::Vec3& up);

    bool assignNewCenter(osgViewer::Viewer* viewer,float x,float y);
    osg::Vec3d eyePosition()const
    {
        return osg::Vec3d(0,0,0)*getMatrix();
        //   return _center*getMatrix();
    }

    /** For the give mouse movement calculate the movement of the camera.
            Return true is camera has moved and a redraw is required.*/
    bool calcMovement();

    void trackball(osg::Vec3& axis,float& angle, float p1x, float p1y, float p2x, float p2y);
    float tb_project_to_sphere(float r, float x, float y);


    /** Check the speed at which the mouse is moving.
            If speed is below a threshold then return false, otherwise return true.*/
    bool isMouseMoving();

    // Internal event stack comprising last two mouse events.
    osg::ref_ptr<const GUIEventAdapter> _ga_t1;
    osg::ref_ptr<const GUIEventAdapter> _ga_t0;

    osg::ref_ptr<osg::Node>       _node;
    osgViewer::Viewer* viewer;
    double _modelScale;
    double _minimumZoomScale;
    double _maxZoomScale;

    bool _thrown;
    void Pan(float x,float y);
    osg::Vec3d   _center;
    osg::Vec3d   _targetCenter;

    double       _distance;
    float        _trackballSize;
    typedef double Angle;
    Angle _targetLatitude;
    Angle _targetLongitude;
    double _targetDistance;
    double _fov;
    double _dt,_t0;
    double _deltaZ;
    double _homeDist;

    double _targetAltitude;
    void SlerpToTargetOrientation(double percent);

    void ComputeTargetDistance( double altitude, Angle tilt );
    Angle _targetHeading;
    Angle _targetBank;
    bool cameraTwistLock;
    Angle Altitude;
    Angle _targetTilt;
    Angle _targetFov;
    float mouseDownStartPositionX;
    float mouseDownStartPositionY;


    bool keyHandle(const GUIEventAdapter& ea ,GUIActionAdapter& us);
    //Camera

    double _XMomentum;
    double _YMomentum;
    void _frame(const GUIEventAdapter& ea);
    
    float cameraSlerpPercentage;
    float _minalt;
    float tiltAngleOffset;
    float cameraPanAcceleration ;
    float cameraZoomAcceleration ;
    float cameraZoomAnalogFactor ;
    float cameraZoomStepKeyboard ;
    float _panKeyDist;
    float cameraRotationSpeed ;
    //osgSim::HeightAboveTerrain hat;
    void setLabel(const std::string& name)
    {
        //if (_updateText.get()) _updateText->setText(name);
    }

    void pick(const osgGA::GUIEventAdapter& ea);
    float _x,_y,_tilt;
    void Zoom(float percent);
    osg::Quat	 _targetOrientation;
    osg::Quat	 _tiltOrientation;
    osg::Quat	 _targetTiltOrientation;
    osg::Quat	 m_Orientation;
    osg::Matrixd _matrix;

    osg::Matrixd *_inv_loaded_mat;
    osg::Matrixd _invMatrix;
    osg::ref_ptr<osgText::Text>  _updateText;
    osg::Matrixd *_loaded_mat;
    bool _invertMouse;
    osg::Geometry *old_toggle_geo;
    int old_toggle_idx;
    osg::Timer_t lastCheckTimestamp;
    float last_m_x,last_m_y;
    osg::Vec4 old_toggle_color[3];


};
void toggleScribe(osg::Group* parent, osg::Node* node);

#endif

