#include "WWManipulator.hpp"
#include <osg/Quat>
#include <osg/Notify>
#include <osg/BoundsChecking>
#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>
#include <osgUtil/IntersectVisitor>


#include <sstream>

#include <osg/Projection>
#include <osg/MatrixTransform>
#include <osg/Camera>
#include <osg/io_utils>
#include <osg/ComputeBoundsVisitor>
#include <iostream>
#include <osg/LineWidth>
#include <osg/PolygonMode>
#include <osg/ShapeDrawable>
using namespace std;
using namespace osg;
using namespace osgGA;
osg::Node *old=NULL;
struct MyReadCallback : public osgUtil::IntersectionVisitor::ReadCallback
{
  virtual osg::Node* readNodeFile(const std::string& filename)
  {
    return osgDB::readNodeFile(filename);
  }
};


WorldWindManipulatorNew::WorldWindManipulatorNew(osgViewer::Viewer* viewer, osgText::Text *text, bool invertMouse,osg::Matrixd *mat) :viewer(viewer), _updateText(text), _loaded_mat(mat) , _invertMouse(invertMouse),old_toggle_geo(NULL),old_toggle_idx(0)
{

  if(mat){
    _inv_loaded_mat =new osg::Matrixd;
    _inv_loaded_mat->invert(*_loaded_mat);
  }
  _modelScale = 0.01f;
  _minimumZoomScale = 0.5f;
  _maxZoomScale = 1000.0f;
  _thrown = false;
  cameraTwistLock=true;
  _distance = 1.0f;
  _trackballSize = 0.8f;
  _homeDist=-1.0;
  _deltaZ=0.0;
  _dt=0.0;
  _t0=0.0;
  lastCheckTimestamp=osg::Timer::instance()->tick();
  _tilt=0.0;
  _targetTilt= 0.0;
  tiltAngleOffset=5.0;
  // Set to either Inertia or Standard slerp value
  cameraSlerpPercentage = 0.005f;

  _panKeyDist=0.25;
  cameraZoomAcceleration = 10.0;
  cameraZoomAnalogFactor = 2.0;
  cameraZoomStepKeyboard = 0.15;
  cameraRotationSpeed = 1000.0;
  cameraPanAcceleration=-0.6;
  _minalt=0.0;
}


WorldWindManipulatorNew::~WorldWindManipulatorNew()
{
}


void WorldWindManipulatorNew::setNode(osg::Node* node)
{
  _node = node;
  if (_node.get()){
    const osg::BoundingSphere& boundingSphere=_node->getBound();
    _modelScale = boundingSphere._radius;
  }
  if (getAutoComputeHomePosition()) computeHomePosition();
  computePosition(_homeEye, _homeCenter, _homeUp);
  _homeDist=_distance;
  _targetOrientation =m_Orientation;
  _targetCenter=_center;
  _targetTilt=_tilt;
  _targetDistance = _distance;
  _maxZoomScale = _distance;
}


const osg::Node* WorldWindManipulatorNew::getNode() const
{
  return _node.get();
}


osg::Node* WorldWindManipulatorNew::getNode()
{
  return _node.get();
}


void WorldWindManipulatorNew::home(double /*currentTime*/)
{
  
  if (getAutoComputeHomePosition()) computeHomePosition();
  computePosition(_homeEye, _homeCenter, _homeUp);
  _targetOrientation =m_Orientation;
  _targetCenter=_center;
  _targetDistance = _distance;
  _thrown = false;
  _targetTilt=_tilt;
}

void WorldWindManipulatorNew::home(const GUIEventAdapter& ea ,GUIActionAdapter& us)
{
  home(ea.getTime());
  us.requestRedraw();
  us.requestContinuousUpdate(false);
}


void WorldWindManipulatorNew::init(const GUIEventAdapter& ,GUIActionAdapter& )
{
  flushMouseEventStack();
}


void WorldWindManipulatorNew::getUsage(osg::ApplicationUsage& usage) const
{
  usage.addKeyboardMouseBinding("WorldWind: Space","Reset the viewing position to home");
  usage.addKeyboardMouseBinding("WorldWind: +","When in stereo, increase the fusion distance");
  usage.addKeyboardMouseBinding("WorldWind: -","When in stereo, reduce the fusion distance");
}

bool WorldWindManipulatorNew::handle(const GUIEventAdapter& ea,GUIActionAdapter& us)
{
  switch(ea.getEventType())
    {
    case(GUIEventAdapter::FRAME):
      _frame(ea);          
      if (_thrown)
	{
	  if (calcMovement()) us.requestRedraw();
	}
      return false;
    default:
      break;
    }

  if (ea.getHandled()) return false;

  switch(ea.getEventType())
    {
    
    case(GUIEventAdapter::DOUBLECLICK):
    {
      //if(ea.getButtonMask() ==GUIEventAdapter::LEFT_MOUSE_BUTTON){
      {osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&us);
	if(viewer)
	  assignNewCenter(viewer,ea.getX(),ea.getY());
	
      }
      return false;
    }    

    case(GUIEventAdapter::PUSH):
      {
	//if(ea.getButtonMask() ==GUIEventAdapter::MIDDLE_MOUSE_BUTTON){
	//  pick(ea);
	//  return true;
	//}
	flushMouseEventStack();
	addMouseEvent(ea);
	mouseDownStartPositionX=ea.getX();
	mouseDownStartPositionY=ea.getY();

	if (calcMovement()) us.requestRedraw();
	us.requestContinuousUpdate(false);
	_thrown = false;
	return true;
      }

    case(GUIEventAdapter::RELEASE):
      { flushMouseEventStack();
	if (ea.getButtonMask()==0)
	  {
            
	    double timeSinceLastRecordEvent = _ga_t0.valid() ? (ea.getTime() - _ga_t0->getTime()) : DBL_MAX;
	    if (timeSinceLastRecordEvent>0.02) flushMouseEventStack();

	    if (isMouseMoving())
	      {
		if (calcMovement())
		  {
		    us.requestRedraw();
		    us.requestContinuousUpdate(true);
		    _thrown = true;
		  }
	      }
	    else
	      {
		flushMouseEventStack();
		addMouseEvent(ea);
		if (calcMovement()) us.requestRedraw();
		us.requestContinuousUpdate(false);
		_thrown = false;
	      }

	  }
	else
	  {
	    flushMouseEventStack();
	    addMouseEvent(ea);
	    if (calcMovement()) us.requestRedraw();
	    us.requestContinuousUpdate(false);
	    _thrown = false;
	  }
	return true;
      }

    case(GUIEventAdapter::DRAG):
      {
	addMouseEvent(ea);
	if (calcMovement()) us.requestRedraw();
	us.requestContinuousUpdate(false);
	_thrown = false;
	return true;
      }

    case(GUIEventAdapter::MOVE):
      {
	last_m_x=ea.getX();
	last_m_y=ea.getY();
	return false;
      }
    case(osgGA::GUIEventAdapter::SCROLL):
      {
	float val=0.0;
	if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_UP) val=.3;
	else if(ea.getScrollingMotion() == osgGA::GUIEventAdapter::SCROLL_DOWN)
	  val=-.3;
	
	Zoom(val);
	return false;
      }
    case(GUIEventAdapter::KEYDOWN):
	  
      return keyHandle(ea,us);
    case(GUIEventAdapter::FRAME):
      if (_thrown){
	if (calcMovement()) us.requestRedraw();
      }
      return false;
    default:
      return false;
    
    }
  return false;
}


void WorldWindManipulatorNew::SlerpToTargetOrientation(double percent){
  
 
  percent *= _dt*1000;
  m_Orientation.slerp(percent,m_Orientation, _targetOrientation);  
  
  _tilt += (_targetTilt - _tilt)*percent;
  _distance += (_targetDistance - _distance)*percent;
  _center += ( _targetCenter -_center)*percent;
 
}
void WorldWindManipulatorNew::_frame(const GUIEventAdapter& ea){
  double t1 = ea.getTime();
  if( _t0 == 0.0 )
    {
      _t0 = ea.getTime();
      _dt = 0.0;
    }
  else
    {
      _dt = t1 - _t0;
      _t0 = t1;
    }
   

  
  SlerpToTargetOrientation(cameraSlerpPercentage);
  
  _invMatrix=osg::Matrixd::translate(0.0,0.0,_distance)* osg::Matrixd::rotate(osg::Quat(osg::DegreesToRadians(_tilt),osg::Vec3d(1,0,0)))*
    osg::Matrixd::rotate(m_Orientation)*osg::Matrixd::translate(_center);
  _matrix.invert(_invMatrix);
  
 
}

bool WorldWindManipulatorNew::isMouseMoving()
{

  if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

  static const float velocity = 0.1f;

  float dx = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
  float dy = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();
  float len = sqrtf(dx*dx+dy*dy);
  float dt = _ga_t0->getTime()-_ga_t1->getTime();

  return (len>dt*velocity);
}
bool WorldWindManipulatorNew::keyHandle(const GUIEventAdapter& ea ,GUIActionAdapter& us){
  int key=ea.getKey();
  FILE *fp;
  switch(key){
  case GUIEventAdapter::KEY_Space:
    
    flushMouseEventStack();
    _thrown = false;
    home(ea,us);
    return true;
    
  case GUIEventAdapter::KEY_Up:
    Pan(0,-_panKeyDist);
    return true;
  case GUIEventAdapter::KEY_Down:
    Pan(0,_panKeyDist);
    return true;
  case GUIEventAdapter::KEY_Left:
    Pan(_panKeyDist,0);
    return true;
  case GUIEventAdapter::KEY_Right:
    Pan(-_panKeyDist,0);
    return true;
 case 'e':
    Zoom(10);
    return true;
  case 'r': 
    Zoom(-10);
    return true;
  case '%':
   fp=fopen("disp.xf","w");
   for(int i=0; i<16; i++)
     fprintf(fp,"%f ",_matrix(i/4,i%4));
   fclose(fp);
   cout << _matrix <<endl;
   return true;
  default:
    return false;
  }
}
void WorldWindManipulatorNew::flushMouseEventStack()
{
  _ga_t1 = NULL;
  _ga_t0 = NULL;
}


void WorldWindManipulatorNew::addMouseEvent(const GUIEventAdapter& ea)
{
  _ga_t1 = _ga_t0;
  _ga_t0 = &ea;
}

void WorldWindManipulatorNew::setByMatrix(const osg::Matrixd& matrix)
{
  osg::Vec3 lookVector(- matrix(2,0),-matrix(2,1),-matrix(2,2));
  osg::Vec3 eye(matrix(3,0),matrix(3,1),matrix(3,2));
  
  _invMatrix = matrix;
  _matrix.invert( _invMatrix );
  double cur_hat=0.0;
  _center = osg::Vec3(0.0f,0.0f,cur_hat+_distance)*matrix;
  m_Orientation = matrix.getRotate();
  _targetOrientation =m_Orientation;
  _targetCenter=_center;
  _targetDistance = _distance;
 
  
}

osg::Matrixd WorldWindManipulatorNew::getMatrix() const
{

  if(_loaded_mat)
    return   *_inv_loaded_mat ;
  else
    return _invMatrix;
 
}

osg::Matrixd WorldWindManipulatorNew::getInverseMatrix() const
{

  if(_loaded_mat){
    return *_loaded_mat;
  }
  else
    return _matrix;
}

void WorldWindManipulatorNew::computePosition(const osg::Vec3& eye,const osg::Vec3& center,const osg::Vec3& up)
{
 
  osg::Vec3 lv(center-eye);

  osg::Vec3 f(lv);
  f.normalize();
  osg::Vec3 s(f^up);
  s.normalize();
  osg::Vec3 u(s^f);
  u.normalize();
    
  osg::Matrix rotation_matrix(s[0],     u[0],     -f[0],     0.0f,
			      s[1],     u[1],     -f[1],     0.0f,
			      s[2],     u[2],     -f[2],     0.0f,
			      0.0f,     0.0f,     0.0f,      1.0f);
                   
  _center = center;
  _tilt=0.0;
  _distance = lv.length();
  m_Orientation = rotation_matrix.getRotate().inverse();
  _invMatrix=osg::Matrixd::translate(0.0,0.0,0/*dist*/)*osg::Matrixd::rotate(osg::Quat(osg::DegreesToRadians(_tilt),osg::Vec3d(1,0,0)))*osg::Matrixd::rotate(m_Orientation)*osg::Matrixd::translate(_center);
  _matrix.invert(_invMatrix);
}

bool WorldWindManipulatorNew::assignNewCenter(osgViewer::Viewer* viewer,float x,float y)
{
 osgUtil::LineSegmentIntersector::Intersections intersections;

    std::string gdlist="";
    if (viewer->computeIntersections(x,y,intersections))
    {

      if (intersections.size() > 0){
	
	osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
	
	if(!hitr->nodePath.empty()){

	  _minalt=0.0;

	 
	  double zoom_dist=10.0;
	  osg::Vec3d gp= hitr->getWorldIntersectPoint();
	  if(_targetDistance > zoom_dist)
	    _targetDistance=zoom_dist;
	  _targetCenter =gp; 
	  return true;
	}
      }
    }
    return false;
}
bool WorldWindManipulatorNew::calcMovement()
{
  // return if less then two events have been added.
  if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;


  float deltaXNormalized = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
  float deltaYNormalized = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();
  float distance = sqrtf(deltaXNormalized*deltaXNormalized +
			 deltaYNormalized*deltaYNormalized);
  // return if movement is too fast, indicating an error in
  // event values or change in screen.
  if (distance>0.5)
    {
      return false;
    }
    
  // return if there is no movement.
  if (distance==0.0f)
    {
      return false;
    }

  unsigned int buttonMask = _ga_t1->getButtonMask();
  bool isMouseLeftButtonDown =  (buttonMask & GUIEventAdapter::LEFT_MOUSE_BUTTON) != 0;
  bool isMouseRightButtonDown = (buttonMask & GUIEventAdapter::RIGHT_MOUSE_BUTTON) != 0;

   
  
  if (isMouseLeftButtonDown && !isMouseRightButtonDown){
    //Panning 
    double x=deltaXNormalized;
    double y=deltaYNormalized;//*-25*(log(max(_distance,2.5)));
    Pan( x, y );
      
  }else if (!isMouseLeftButtonDown && isMouseRightButtonDown){
    //Tilt and Heading
      
    // Heading
    double headingDelta = ( deltaXNormalized * cameraRotationSpeed);
    if(_invertMouse)
     headingDelta = -headingDelta;
    RotationHeading( headingDelta);
      
    // tilt
    double tiltDelta = ( -deltaYNormalized* 0.1*cameraRotationSpeed);
    if(_invertMouse)
      tiltDelta = -tiltDelta;
    _targetTilt += tiltDelta;
    if(_targetTilt > 90.0 - tiltAngleOffset)
      _targetTilt =90.0-tiltAngleOffset;
    if(_targetTilt < 0.0 +tiltAngleOffset)
      _targetTilt = 0.0+tiltAngleOffset;
      
    return true;
  }
  else {
    // Both buttons (zoom)
    if(fabs(deltaYNormalized) > 1e-5)
      Zoom( -deltaYNormalized*cameraZoomAnalogFactor );
    return true;   
  }
  return false;
}

void WorldWindManipulatorNew::Zoom(float percent){
  //  if(percent > 0)
  //RecomputeTerrainIntersection();
  double tmp=_targetDistance;
  if(percent>0){
    tmp /= 1.0f + percent;
    if(tmp < _minimumZoomScale + _minalt)
      return;
  }
  else{
    
    tmp *= 1.0f - percent;
   
  }
  if(tmp < _minimumZoomScale || tmp > _maxZoomScale)
    return;
  else
    _targetDistance =tmp;

}


void WorldWindManipulatorNew::Pan(float x, float y){
  
  // pan model.
 
  //x*= (-25*(log(max(_distance,2.5))));
  //y*= (-25*(log(max(_distance,2.5))));

  float scale = cameraPanAcceleration*_distance;
  
  osg::Matrix rotation_matrix;
  rotation_matrix.makeRotate(m_Orientation);
  
  osg::Vec3 dv(x*scale,y*scale,0.0f);

  _targetCenter += dv*rotation_matrix;
  //cout << "Hit " <<hat.computeHeightAboveTerrain(_node.get(),_center) << endl;


}

class DrawnBoundBox : public osg::Geode
{
public:
  DrawnBoundBox( const osg::BoundingBox &bb, const osg::Matrix &mat ):
    _bb(bb)
  {
    osg::ref_ptr<osg::Vec3Array> coords = new osg::Vec3Array;

    double x0 = _bb._min[0];
    double y0 = _bb._min[1];
    double z0 = _bb._min[2];

    double x1 = _bb._max[0];
    double y1 = _bb._max[1];
    double z1 = _bb._max[2];

    coords->push_back( osg::Vec3(x0,y0,z0) * mat);
    coords->push_back( osg::Vec3(x1,y0,z0) * mat);
    coords->push_back( osg::Vec3(x1,y0,z1) * mat);
    coords->push_back( osg::Vec3(x0,y0,z1) * mat);
    coords->push_back( osg::Vec3(x0,y0,z0) * mat);
    coords->push_back( osg::Vec3(x0,y1,z0) * mat);
    coords->push_back( osg::Vec3(x0,y1,z1) * mat);
    coords->push_back( osg::Vec3(x0,y0,z1) * mat);
    coords->push_back( osg::Vec3(x1,y0,z1) * mat);
    coords->push_back( osg::Vec3(x1,y1,z1) * mat);
    coords->push_back( osg::Vec3(x0,y1,z1) * mat);
    coords->push_back( osg::Vec3(x0,y1,z0) * mat);
    coords->push_back( osg::Vec3(x1,y0,z0) * mat);
    coords->push_back( osg::Vec3(x1,y1,z0) * mat);
    coords->push_back( osg::Vec3(x1,y1,z1) * mat);

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    geometry->setVertexArray( coords.get() );

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4( 1, 0, 0,1 ));

    geometry->setColorArray( colors.get() );
    geometry->setColorBinding( osg::Geometry::BIND_OVERALL );

    geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, coords->size()));

    osg::ref_ptr<osg::StateSet> sset = new osg::StateSet;
    sset->setMode( GL_LIGHTING, osg::StateAttribute::OFF );

    osg::LineWidth *lw = new osg::LineWidth(1.5);
    sset->setAttributeAndModes( lw);

    geometry->setStateSet( sset.get()) ;

    addDrawable( geometry.get());
  }

private:
  osg::BoundingBox _bb;
};
class AddBoundingBoxToDrawables: public osg::NodeVisitor
{
public:
  AddBoundingBoxToDrawables(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN )
  {
    _group = new osg::Group;
  }

  osg::Group *getGroup() { return _group.get(); }

  virtual void apply(osg::Geode& geode)
  {
    for( unsigned int i= 0; i < geode.getNumDrawables(); i++ )
      {
	osg::Drawable *drawable = geode.getDrawable(i);
	osg::BoundingBox bb = drawable->getBound();

	osg::Matrix mat = osg::computeLocalToWorld( getNodePath());
	_group->addChild( new DrawnBoundBox( bb, mat ));
      }
    traverse(geode);
  }

private:
  osg::ref_ptr<osg::Group> _group;
};

void toggleScribe(osg::Group* parent, osg::Node* node) {
  if (!parent || !node) {printf("Fuck\n");return;
  }
 /* std::cout<<"  parent "<<parent->className()<<std::endl;

  osgFX::Scribe* parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent);
  if (!parentAsScribe)
    {
      // node not already picked, so highlight it with an osgFX::Scribe
      osgFX::Scribe* scribe = new osgFX::Scribe();
      scribe->addChild(node);
      parent->replaceChild(node,scribe);
    }
  else
    {
      // node already picked so we want to remove scribe to unpick it.
      osg::Node::ParentList parentList = parentAsScribe->getParents();
      for(osg::Node::ParentList::iterator itr=parentList.begin();
	  itr!=parentList.end();
	  ++itr)
	{
	  (*itr)->replaceChild(parentAsScribe,node);
	}
    }
*/
    }
void WorldWindManipulatorNew::RecomputeTerrainIntersection(){
  // need to recompute the itersection point along the look vector.
  //osg::setNotifyLevel(osg::INFO);
  /*osg::Timer_t current_tick = osg::Timer::instance()->tick();

    if( current_tick- lastCheckTimestamp < 20000){
    lastCheckTimestamp=current_tick;
    return;
  }
  else{
    lastCheckTimestamp=current_tick;
  }
  */

  if(assignNewCenter(viewer,last_m_x,last_m_y))
    return;


  if (_node.valid()){
    
    osg::Vec3d upVector (0.0, 1.0, 0.0);
    osg::Vec3d start = _center + upVector *50;
    osg::Vec3d end = _center- upVector * 50;      
    if(start == end)
      return;
    osgUtil::LineSegmentIntersector* linepicker= new osgUtil::LineSegmentIntersector(osgUtil::Intersector::VIEW,start,end);
    
    osgUtil::IntersectionVisitor iv(linepicker);
    
    iv.setTraversalMask(_intersectTraversalMask);
    _node->accept(iv);

    if (linepicker->containsIntersections()){
      osgUtil::LineSegmentIntersector::Intersections& lintersections =  linepicker->getIntersections();
      osgUtil::LineSegmentIntersector::Intersections::iterator hitr = lintersections.begin();
     
      if(!hitr->nodePath.empty()){
	//	printf("hit\n");
	_minalt=0.0;
	osg::Vec3d gp= hitr->getWorldIntersectPoint();
	double newZ=gp[1];
	double oldZ=_center[1];
	if(_center[1] != gp[1]){
	  _deltaZ=oldZ-newZ;
	  _center[1]= newZ;
	  _distance-=_deltaZ;
	  _targetDistance=_distance;
	  _targetCenter[1] =_center[1]; 
	}
      }
    }//else{

      
    
    /* else{
      _minalt=_modelScale*.05;
   
      if(_distance > (_modelScale*.05))   {
	osgUtil::PolytopeIntersector*picker=NULL;
	
	osg::Polytope::PlaneList planeList ; 
	osg::Vec3d tmp=osg::Vec3d(_center[0],_center[1]+_deltaZ,_center[2]);
	osg::Matrix trans=osg::Matrixd::translate(tmp);
	
	double box_size=5.0;
	Polytope pt;
	BoundingBox bbox(-box_size*2,-10*box_size,0,
			 box_size*2, 0, 2*box_size);
	pt.setToBoundingBox(bbox    );
	
	pt.transform(trans);
	
//	if(dynamic_cast<osg::Group *>( _node.get())){  
	//  osg::Node *tp=new DrawnBoundBox(  bbox,trans);
//	  if(old)	      
	//     dynamic_cast<osg::Group *>( _node.get()) ->replaceChild(old,tp);
	//  else
	 ///   dynamic_cast<osg::Group *>( _node.get()) ->addChild(tp);
	 // old=tp;
//	}
	
	picker= new osgUtil::PolytopeIntersector( osgUtil::Intersector::VIEW, pt );
	
	
	
	
	osgUtil::IntersectionVisitor iv(picker);
	
      iv.setTraversalMask(_intersectTraversalMask);
      _node->accept(iv);
      
      bool hitFound = false;
      if (picker->containsIntersections()){
	osgUtil::PolytopeIntersector::Intersections& intersections =  picker->getIntersections();
	
	osgUtil::PolytopeIntersector::Intersections::iterator hitr = intersections.begin();
     
	_minalt=0.0;
     
     
	osg::NodePath nodePath = hitr->nodePath;
	osg::Node* node = 0;
	osg::Group* parent = 0;
	node = (nodePath.size()>=1)?nodePath[nodePath.size()-1]:0;
	parent = (nodePath.size()>=2)?dynamic_cast<osg::Group*>(nodePath[nodePath.size()-2]):0;
	if (node && node != old) {
	  _minalt=0.0;
	  //	  printf("Contain intersection\n");
	  //	toggleScribe(parent, node);
	  //osg::Vec3d gp2=hitr->getWorldIntersectPoint();
	  osg::Vec3 gp=node->getBound().center();
	  //	cout << "Bounds "<< -gp[2] << " " <<_center[1] <<"\n";// << gp2 << endl;
	  double newZ=-gp[2]; //gp[1];
	  //  std::cout<<"  Hits " << newZ << endl; 
	  double oldZ=_center[1];
	  if(_center[1] != -gp[2]){
	    _deltaZ=oldZ-newZ;
	    //	cout << "Delta " << _deltaZ; 
	    _center[1]= newZ;
	    _distance-=_deltaZ;
	    _targetDistance=_distance;
	    _targetCenter[1] =_center[1]; 
	  }
	  hitFound=true;
	}
	picker->reset();
      }
      
      //_minalt=5.0;
      }
   
     
    }*/
  }
}
void WorldWindManipulatorNew::RotationHeading(float y){
 
  osg::Quat heading(osg::DegreesToRadians(y),osg::Vec3d(0,0,1));
  osg::Quat rot=heading*m_Orientation;
  _targetOrientation = rot;
}


void WorldWindManipulatorNew::pick( const osgGA::GUIEventAdapter& ea)
{
  osgUtil::LineSegmentIntersector::Intersections intersections;

  std::string gdlist="";
  if (viewer->computeIntersections(ea.getX(),ea.getY(),intersections))
    {
      for(osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
	  hitr != intersections.end();
	  ++hitr)
        {
	  std::ostringstream os;
	  if (!hitr->nodePath.empty() && !(hitr->nodePath.back()->getName().empty()))
            {
	      // the geodes are identified by name.
	      os<<"Object \""<<hitr->nodePath.back()->getName()<<"\""<<std::endl;
	      osg::Node* node = 0;
	      osg::Group* parent = 0;
		
	      osg::NodePath nodePath = hitr->nodePath;
	      node = (nodePath.size()>=1)?nodePath[nodePath.size()-1]:0;
	      parent = (nodePath.size()>=2)?dynamic_cast<osg::Group*>(nodePath[nodePath.size()-2]):0;
	      //toggleScribe(parent, node);
	      osg::ComputeBoundsVisitor cbbv;
	      hitr->nodePath.back()->accept(cbbv);
	      osg::Polytope test;
	      cbbv.getPolytope(test);
	      osg::Polytope::PlaneList l = test.getPlaneList();
	      for(unsigned int i=0; i < l.size(); i++)
		cout << "polytope " << l[i]<<endl;
		
	    }
	  else if (hitr->drawable.valid())
            {
	      os<<"Object \""<<hitr->drawable->className()<<"\""<<std::endl;
            }
	   
	  //os<<"start="<<start_segment<<"\tend="<<end_segment<<"\tupVector="<<getUpVector(coordinateFrame)<<std::endl;
	  os<<" ("<< _distance<<")"<<"\n";
	  os <<"Center " << _center <<"\n";
	  os<<hitr->getWorldIntersectPoint() <<"\n";

	  os<<"        local coords vertex("<< hitr->getLocalIntersectPoint()<<")"<<"  normal("<<hitr->getLocalIntersectNormal()<<")"<<std::endl;
	  os<<"        world coords vertex("<< hitr->getWorldIntersectPoint()<<")"<<"  normal("<<hitr->getWorldIntersectNormal()<<")"<<std::endl;
	  const osgUtil::LineSegmentIntersector::Intersection::IndexList& vil = hitr->indexList;
	  for(unsigned int i=0;i<vil.size();++i)
            {
	      os<<"        vertex indices ["<<i<<"] = "<<vil[i]<<std::endl;
            }
    
	  gdlist += os.str();
        }
    }
  setLabel(gdlist);
}
