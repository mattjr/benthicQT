#ifndef SCREENTOOLS_H
#define SCREENTOOLS_H
#include <osg/Geometry>
#include <osgUtil/Optimizer>
#include <osgViewer/Viewer>
#include <osg/CameraNode>
#include <osg/FrontFace>
#include <osg/io_utils>
#include <sstream>
#include <osgText/Text>
#include <osgDB/FileUtils>
#include "WWManipulator.hpp"
#include <osg/TexMat>
#include <osg/ShadeModel>
#include <osg/LineWidth>
#include <iostream>
#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osg/CullFace>
#include "MeshFile.h"
#include <stdio.h>
#include <osgSim/ScalarsToColors>
#include <osgSim/ColorRange>
#include <osgSim/ScalarBar>
using namespace std;
extern osg::Geode* measure_geode;
extern bool measuring_tool_on;
extern bool oldmesh;

class GetUniformStateSetsListVisitor : public osg::NodeVisitor
{
public:

  GetUniformStateSetsListVisitor():
    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
  {}

  virtual void apply(osg::Node& node)
  {
    if (node.getStateSet()) apply(*node.getStateSet());
    traverse(node);
  }
    
  virtual void apply(osg::Geode& node)
  {
    if (node.getStateSet()) apply(*node.getStateSet());
        
    for(unsigned int i=0;i<node.getNumDrawables();++i)
      {
	osg::Drawable* drawable = node.getDrawable(i);
	if (drawable && drawable->getStateSet()) apply(*drawable->getStateSet());
      }
        
    traverse(node);
  }
    
  virtual void apply(osg::StateSet& stateset)
  {
        
    if(stateset.getUniformList().size())
      _stateSetList.push_back(&stateset);
  }
    
  std::vector< osg::StateSet * >    getStateSetList(){ return _stateSetList;} 

    
    
  std::vector< osg::StateSet * >                _stateSetList;
  
    
};

#define NUM_SHADER_MODES 5
class ShaderEventHandler : public osgGA::GUIEventHandler
{
public:
  string soStr[NUM_SHADER_MODES];
 
  ShaderEventHandler(osg::StateSet *stateset,osgText::Text *shaderText,int shaderOut=-1):
    stateset(stateset),shaderText(shaderText),shaderOut(shaderOut) { 
    soStr[0]= "0 Pass Through Closest Texture No Blend";
    soStr[1]=    "1 Freq Band Blending";
    soStr[2]=    "2 Straight Avg";
    soStr[3]=   "3 Std Err";
    soStr[4]=   "4 three freq blend";
    char tmp[255];
    osg::Uniform* uniform = 0;
    uniform = stateset->getUniform("shaderOut"); 
    if(uniform ){
      if(shaderOut == -1)
	uniform->get(shaderOut);
      else {
	uniform->set(shaderOut);
	printf("Setting %d\n",shaderOut);
      }
    }
    uniform = stateset->getUniform("weights"); 
    if(uniform)
      uniform->get(weights);
    
    sprintf(tmp,"Shader out %s %.3f %.3f %.3f\n",soStr[shaderOut].c_str(),weights[0],weights[1],weights[2]);    
    shaderText->setText(tmp);
    cout <<tmp<<endl;
  }
  virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
  {
	 
    return false;         
    if (!stateset) return false;
    osg::Uniform* uniform = 0;
    switch(ea.getEventType())
      {
      case(osgGA::GUIEventAdapter::MOVE):
      case(osgGA::GUIEventAdapter::DRAG):
	{
	  //float v = ea.getY()*0.5f+0.5f;
                 
	  /*if (_updateTransparency && (uniform = stateset->getUniform("shaderOut"))) uniform->set(((int)v*5)%5);
	    if (_updateAlphaCutOff && (uniform = stateset->getUniform("alphaCutOff"))) uniform->set(v);
	    if (_updateSampleDensity && (uniform = stateset->getUniform("sampleDensity"))) uniform->set(powf(v,5));*/
	  break;
	}
      case(osgGA::GUIEventAdapter::KEYDOWN):
	{
	  if (ea.getKey()=='t') {
	    shaderOut = ((shaderOut+1) % (NUM_SHADER_MODES));
		    
	    uniform = stateset->getUniform("shaderOut"); 
	    if(uniform){
	      uniform->set(shaderOut);
		  
	    }
 
	  }
	  if (ea.getKey()=='r') {
	    shaderOut = (shaderOut-1);
	    if(shaderOut < 0)
	      shaderOut=NUM_SHADER_MODES-1;
	    uniform = stateset->getUniform("shaderOut");    if(uniform)uniform->set(shaderOut);
 
	  }
	  bool updateWeight = false;
	  double deltaW= (ea.getModKeyMask() & ea.MODKEY_SHIFT) ? 0.001 : 0.01;
		  
	  if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_End){
	    weights[0]-=deltaW;
	    updateWeight=true;
	  }
	  if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Down){
	    weights[1]-=deltaW;
	    updateWeight=true;
	  }
	  if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Page_Down){
	    weights[2]-=deltaW;
	    updateWeight=true;
	  }

	  if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Left){
	    weights[0]+=deltaW;
	    updateWeight=true;
	  }
	  if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Begin){
	    weights[1]+=deltaW;
	    updateWeight=true;
	  }
	  if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Right){
	    weights[2]+=deltaW;
	    updateWeight=true;
	  }
	  //if (ea.getKey()=='a') _updateAlphaCutOff = true;
	  //if (ea.getKey()=='d') _updateSampleDensity = true;
	  if(updateWeight){
	    if(weights[0] <=0.0)
	      weights[0]=0.0;
	    if(weights[1] <=0.0)
	      weights[1]=0.0;
	    if(weights[2] <=0.0)
	      weights[2]=0.0;
	    if(stateset)
	      uniform = stateset->getUniform("weights");   
	    if(uniform)
	      uniform->set(weights);
	  }
	  char tmp[255];
	  sprintf(tmp,"Shader out %s %.3f %.3f %.3f\n",soStr[shaderOut].c_str(),weights[0],weights[1],weights[2]);
		    
	  shaderText->setText(tmp);
	  break;
	}
      case(osgGA::GUIEventAdapter::KEYUP):
	{
	  //if (ea.getKey()=='t') _updateTransparency = false;
	  //if (ea.getKey()=='a') _updateAlphaCutOff = false;
	  //if (ea.getKey()=='d') _updateSampleDensity = false;
	  break;
	}
      default:
	break;
      }
    return false;
  }



  osg::Vec3 weights;
  osg::StateSet *stateset;
  osgText::Text *shaderText;
  int shaderOut;
};
osg::Camera* createBox(int xpos, int ypos,int hud_width,int hud_height);
osgText::Text* createMeasureText(osg::Vec3 start, osg::Vec3 end);

class tex_show_box : public osg::Camera {
public:
  tex_show_box(int xpos, int ypos) : osg::Camera() {
    hud_width=200;
    hud_height=200;
    margin=20;
    createBox(xpos, ypos,
	      hud_width,hud_height);
    createRTTQuad();
  }
  void show(osg::Texture *texture){
    if(texture)
      stateset->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);
  }
  void createBox(int xpos, int ypos,int hud_width,int hud_height){

 
  
  
    this->setViewport(xpos,ypos,hud_width,hud_height);
    // set up the background color and clear mask.
    this->setClearColor(osg::Vec4(0.1f,0.1f,0.3f,1.0f));
    this->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    // the this is going to look at our input quad
    this->setProjectionMatrix(osg::Matrix::ortho2D(0,1,0,1));
    this->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    this->setViewMatrix(osg::Matrix::identity());
    this->setRenderOrder(osg::Camera::POST_RENDER);
  }

protected:
  void createRTTQuad(void){
    top_group = new osg::Group;
    osg::ref_ptr<osg::Geode> quad_geode = new osg::Geode;

    osg::ref_ptr<osg::Vec3Array> quad_coords = new osg::Vec3Array; // vertex coords
    // counter-clockwise
    quad_coords->push_back(osg::Vec3d(0, 0, -1));
    quad_coords->push_back(osg::Vec3d(1, 0, -1));
    quad_coords->push_back(osg::Vec3d(1, 1, -1));
    quad_coords->push_back(osg::Vec3d(0, 1, -1));

    osg::ref_ptr<osg::Vec2Array> quad_tcoords = new osg::Vec2Array; // texture coords
    quad_tcoords->push_back(osg::Vec2(0, 0));
    quad_tcoords->push_back(osg::Vec2(1.0, 0));
    quad_tcoords->push_back(osg::Vec2(1.0, 1.0));
    quad_tcoords->push_back(osg::Vec2(0, 1.0));

    osg::ref_ptr<osg::Geometry> quad_geom = new osg::Geometry;
    osg::ref_ptr<osg::DrawArrays> quad_da = new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4);

    osg::ref_ptr<osg::Vec4Array> quad_colors = new osg::Vec4Array;
    quad_colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));

    quad_geom->setVertexArray(quad_coords.get());
    quad_geom->setTexCoordArray(0, quad_tcoords.get());
    quad_geom->addPrimitiveSet(quad_da.get());
    quad_geom->setColorArray(quad_colors.get());
    quad_geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    stateset = quad_geom->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    stateset->setDataVariance(osg::Object::DYNAMIC);
  
    quad_geode->addDrawable(quad_geom.get());
    
    top_group->addChild(quad_geode.get());
    this->addChild(top_group);
  }
  int hud_width;
  int hud_height;			       
  int margin;

  osg::Group *top_group ;
  osg::StateSet *stateset;
};

class PickHandler : public osgGA::GUIEventHandler {

public: 
  PickHandler(ews::app::model::MeshFile *mf,osg::Group *group=NULL):
    _mf(mf),measure_anchor(NULL),group(group),measure_anchored(false),measure_added(false){
    measureGeom=new osg::Geometry();
    measureGeom->setDataVariance(osg::Object::DYNAMIC);

    measure_vertices = new osg::Vec3Array(2);
    measure_vertices->setDataVariance(osg::Object::DYNAMIC);
    measureGeom->setVertexArray(measure_vertices);
    osg::Vec4Array* colors = new osg::Vec4Array;
    // add a white color, colors take the form r,g,b,a with 0.0 off, 1.0 full on.
    colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
    
    // pass the color array to points geometry, note the binding to tell the geometry
    // that only use one color for the whole object.
    measureGeom->setColorArray(colors);
    measureGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    
    // set the normal in the same way color.
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    measureGeom->setNormalArray(normals);
    measureGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    
    
    // create and add a DrawArray Primitive (see include/osg/Primitive).  The first
    // parameter passed to the DrawArrays constructor is the Primitive::Mode which
    // in this case is LINES (which has the same value GL_LINES), the second
    // parameter is the index position into the vertex array of the first point
    // to draw, and the third parameter is the number of points to draw.
    measureGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,measure_vertices->size()));
     measureGeom->setUseDisplayList(false);
     measure_geode  = new osg::Geode();
     measure_geode->addDrawable(measureGeom);
     measure_geode->setDataVariance(osg::Object::DYNAMIC);
     text_ptr=createMeasureText((*measure_vertices)[0],(*measure_vertices)[1]);
     osg::StateSet *stateset=measure_geode->getOrCreateStateSet();
     stateset->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
     stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
     stateset->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
     osg::ShadeModel *pShadeMdl = new osg::ShadeModel;
     
     pShadeMdl->setMode(osg::ShadeModel::FLAT);
     
     stateset->setAttribute(pShadeMdl, osg::StateAttribute::ON);
     osg::LineWidth* linewidth = new osg::LineWidth();
     linewidth->setWidth(3.0f);
     stateset->setAttributeAndModes(linewidth,osg::StateAttribute::ON); 
     osg::CullFace *_cullFaceState = new osg::CullFace;

     
     measure_geode->getOrCreateStateSet()-> setAttributeAndModes(_cullFaceState, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE);
     measure_geode->setCullingActive(false);
     measure_geode->addDrawable(text_ptr);
     /*     if(group)
       ;   group->addChild(measure_geode);
    else
      fprintf(stderr,"Group is NULL");
     */
  }
        
  ~PickHandler() {}
    
  bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
  void pick_measure(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);
  virtual void pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea);

protected:
  ews::app::model::MeshFile  *_mf;
  osg::Vec3Array *measure_vertices;
  osg::Vec3 *measure_anchor;
  osg::Geometry *measureGeom;
  osg::Group *group;
 
  bool measure_anchored;
  bool measure_added;
  osgText::Text *text_ptr;
};


osg::Camera* createOrthoView(osg::Node* subgraph, const osg::Vec4& clearColour, WorldWindManipulatorNew *om,int screen_width,int screen_height,int hud_width,int hud_height,int hud_margin);
osg::Group* createRTTQuad(osg::Texture *texture);
double computePixelSizeAtDistance(double distance, double fieldOfView, double viewportWidth);
osg::Node *createScaleBar(osgText::Text *textNode,WorldWindManipulatorNew *om,osg::Camera *cam);
#endif
