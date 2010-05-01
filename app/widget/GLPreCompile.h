#ifndef GLPRECOMPILE_H
#define GLPRECOMPILE_H
#include <osgUtil/GLObjectsVisitor>

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
/*      virtual void apply(osg::Geode& node);
      virtual void apply(osg::Drawable& drawable);
      virtual void apply(osg::StateSet& stateset);*/
   };

#endif // GLPRECOMPILE_H
