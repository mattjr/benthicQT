#include "UniformCallback.hpp"
bool switch_state=false;

class SwitchUpdateCallback : public osg::NodeCallback
{
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
        { 
          osg::Switch * the_switch = dynamic_cast<osg::Switch *>(node);
	  if(the_switch){
	    if(switch_state)
	      the_switch->setAllChildrenOn();
	    else
	      the_switch->setAllChildrenOff();
	  }
            traverse(node,nv);
         
        }
};




ShaderSwitchEventHandler::ShaderSwitchEventHandler(osg::Vec3 &weights,
						   int shaderOut,
						   int num_shader_modes):
  weights(weights), shaderOut(shaderOut),num_shader_modes(num_shader_modes),shader_out_uni(NULL),weights_uni(NULL) { 
  
}

   bool ShaderSwitchEventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
  {
        if (!shader_out_uni) return false;
  
    switch(ea.getEventType()){
      case(osgGA::GUIEventAdapter::KEYDOWN):
      {
	if (ea.getKey()=='t') {
        printf("HHHAHA\n");
	  shaderOut = std::min(shaderOut+1,num_shader_modes-1);
	  shader_out_uni->set(shaderOut);
	}
	if (ea.getKey()=='r') {
	  shaderOut = std::max(shaderOut-1,0);	 
	  shader_out_uni->set(shaderOut);
	
	}
	if (ea.getKey()=='l') {
	  switch_toggle();
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
	if(ea.getKey()==osgGA::GUIEventAdapter::KEY_KP_Enter){
	  printf("Weights %f %f %f\n",weights[0],weights[1],weights[2]);
	  printf("Shader Out %d\n",shaderOut);

	}
	if(updateWeight){
	  if(weights[0] <=0.0)
	    weights[0]=0.0;
	  if(weights[1] <=0.0)
	    weights[1]=0.0;
	  if(weights[2] <=0.0)
	      weights[2]=0.0;
	  
	  if(weights_uni)
	    weights_uni->set(weights);
	}
	
      }
    default:
	break;
      }
    return false;
  }
void ShaderSwitchEventHandler::switch_toggle(void){
  

  switch_state= !switch_state;
    
}

 GetUniformStateSetsListVisitor::GetUniformStateSetsListVisitor():
    osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
  {}

 void GetUniformStateSetsListVisitor::apply(osg::Node& node)
  {
    if (node.getStateSet()) apply(*node.getStateSet());
    traverse(node);
  }
    
 void GetUniformStateSetsListVisitor::apply(osg::Geode& node)
  {
    if (node.getStateSet()) apply(*node.getStateSet());
        
    for(unsigned int i=0;i<node.getNumDrawables();++i)
      {
	osg::Drawable* drawable = node.getDrawable(i);
	if (drawable && drawable->getStateSet()) apply(*drawable->getStateSet());
      }
        
    traverse(node);
  }

 void GetUniformStateSetsListVisitor::apply(osg::Switch& node)
  {
  
    node.setUpdateCallback(new SwitchUpdateCallback ());
    traverse(node);
  }
    
void GetUniformStateSetsListVisitor::apply(osg::StateSet& stateset)
  {
    stateset.setDataVariance(osg::Object::DYNAMIC);
    if(stateset.getUniformList().size())
      _stateSetList.push_back(&stateset);
  }

MyReadCallback::MyReadCallback(ShaderSwitchEventHandler &sseh):sseh(sseh){

  /*  shared_shader_out = new osg::Uniform("shaderOut",0);
  shared_shader_out->setDataVariance(osg::Object::DYNAMIC);
  sseh.setShaderOutUniform(shared_shader_out);*/
}
osgDB::ReaderWriter::ReadResult  MyReadCallback:: readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options){
  
  // note when calling the Registry to do the read you have to call readNodeImplementation NOT readNode, as this will
  // cause on infinite recusive loop.
  osgDB::ReaderWriter::ReadResult result = osgDB::Registry::instance()->readNodeImplementation(fileName,options);
  if(result.getNode()){
    GetUniformStateSetsListVisitor uniformListVisitor;

    result.getNode()->accept(uniformListVisitor);
    /*   for(int i=0; i <(int) uniformListVisitor.getStateSetList().size(); i++){
      if(uniformListVisitor.getStateSetList()[i]){
	uniformListVisitor.getStateSetList()[i]->removeUniform("shaderOut");

	   shared_shader_out->setDataVariance(osg::Object::DYNAMIC);
	   uniformListVisitor.getStateSetList()[i]->addUniform(shared_shader_out);
      }
      }*/

  }
 

  return result;
}

   
#if 0 
void ShaderSwitchEventHandler::addStateSet(osg::StateSet *stateset){
  if(stateset){
    uniforms.push_back(stateset->getUniform("shaderOut"));
    printf("found shader out\n");
  }
  
}
  void ShaderSwitchEventHandler::addStateSetVec(std::vector<osg::StateSet*> state_vec){
 

    for(int i=0; i<state_vec.size();i++){
      osg::Uniform *uni= state_vec[i]->getUniform("shaderOut"); 
      uniforms.push_back(uni);
      if(uniforms.back()){
	if(shaderOut == -1)
	  uniforms.back()->get(shaderOut);
	else {
	  uniforms.back()->set(shaderOut);
	}
      }
      }
      }
#endif
