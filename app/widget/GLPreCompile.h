#ifndef GLPRECOMPILE_H
#define GLPRECOMPILE_H
#include <osgUtil/GLObjectsVisitor>
#include <osgUtil/IncrementalCompileOperation>

class  CompileObjects : public osgUtil::GLObjectsVisitor
   {
   public:
      CompileObjects()
      : osgUtil::GLObjectsVisitor(COMPILE_STATE_ATTRIBUTES|COMPILE_DISPLAY_LISTS|CHECK_BLACK_LISTED_MODES)
      {

      }
      ~CompileObjects(){}

      virtual void apply(osg::Node& node){
          osgUtil::GLObjectsVisitor::apply(node);
          qApp->processEvents();

      }

#endif // GLPRECOMPILE_H
