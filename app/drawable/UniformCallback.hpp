
#ifndef UNIFORMCALLBACK_H
#define UNIFORMCALLBACK_H
#include <osgViewer/Viewer>
#include <osg/StateSet>
#include <osg/io_utils>
#include <sstream>
#include <osg/Uniform>
#include <iostream>
#include <osg/MatrixTransform>
#include <osg/PolygonMode>
#include <osg/CullFace>
#include <vector>
#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/LineSegmentIntersector>

class ShaderSwitchEventHandler : public osgGA::GUIEventHandler
{
public:
  ShaderSwitchEventHandler(osg::Vec3 &weights,int shaderOut=0,int num_shader_modes=2);
 
  virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
    void addStateSet(osg::StateSet *stateset);
  void setShaderOutUniform(osg::Uniform *_shader_out_uni){shader_out_uni=_shader_out_uni;shader_out_uni->set(shaderOut);};
 void setWeightUniform(osg::Uniform *_weights_uni){weights_uni=_weights_uni;weights_uni->set(weights);};
  // void  addStateSetVec(std::vector<osg::StateSet*> statesets);
   osg::Vec3 &weights;
  void switch_toggle(void);
  int shaderOut;
  int num_shader_modes;
  osg::Uniform *shader_out_uni,*weights_uni;

};

class MyReadCallback : public osgDB::Registry::ReadFileCallback
{
public:
  MyReadCallback(ShaderSwitchEventHandler &sseh);
  virtual osgDB::ReaderWriter::ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options);
  ShaderSwitchEventHandler &sseh;
  osg::Uniform *shared_shader_out;
};

class GetUniformStateSetsListVisitor : public osg::NodeVisitor
{
public:
  GetUniformStateSetsListVisitor();
  virtual void apply(osg::Node& node);
  virtual void apply(osg::Geode& node);
  virtual void apply(osg::StateSet& stateset);
  virtual void apply(osg::Switch& node);

    
  std::vector< osg::StateSet * >    getStateSetList(){ return _stateSetList;} 

private:
 
  std::vector< osg::StateSet * >                _stateSetList;
  
    
};
extern bool switch_state;
#endif
