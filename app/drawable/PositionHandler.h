/* Copyright 2010 NVIDIA Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Developed by Mustard Seed Software, LLC 
 * http://mseedsoft.com
 */

#ifndef __POSTITIONHANDLER_H
#define __POSITIONHANDLER_H

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgManipulator/Dragger>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/View>
#include "BQTDebug.h"
#include "MeshFile.h"
#include "auv_map_projection.hpp"
namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgGA;
            using namespace osgUtil;
            using namespace osgManipulator;
            using ews::app::model::MeshFile;
            typedef LineSegmentIntersector::Intersections::iterator IntersectIterator;
            
            /**
             * Class responsible for determining which object in a scene graph has been clicked on.
             */
            class PositionHandler :  public osgGA::GUIEventHandler  {
                
            public:
                
                PositionHandler(MeshFile *mf,double latOrigin,double longOrigin) : GUIEventHandler(), activeDragger(NULL),_mf(mf) {
                    projWGS84 =new Local_WGS84_TM_Projection(latOrigin,longOrigin);
                }
                
                /** Handle GUI event. */
                virtual bool handle(const GUIEventAdapter& ea,
                                    GUIActionAdapter& aa, osg::Object* obj, NodeVisitor* nv) { 

                    osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
                    if (view) {
                        
                        switch (ea.getEventType()) {
                                
                            case GUIEventAdapter::MOVE:
                            {
                                LineSegmentIntersector::Intersections intersections;
                                pointerInfo.reset();
                                
                                if (view->computeIntersections(ea.getX(), ea.getY(), intersections)) {
                                    
                                   const osgUtil::LineSegmentIntersector::Intersection& intersection = *(intersections.begin());
                                   osg::Vec3 cursor_pos=intersection.getWorldIntersectPoint();
                                   osg::Vec3d world=cursor_pos;
                                   if(projWGS84){
                                      projWGS84->calc_geo_coords(cursor_pos.x(),cursor_pos.y(),world.x(),world.y());
                                      _mf->updateGlobal(world);


                                  }else
                                      _mf->updateGlobal(cursor_pos);

                                   _mf->updateImage(cursor_pos);


                                }
                            }
                                break;
                                
                            case GUIEventAdapter::RELEASE:
                                if (activeDragger) {
                                    pointerInfo._hitIter = pointerInfo._hitList.begin();
                                    pointerInfo.setCamera(view->getCamera());
                                    pointerInfo.setMousePosition(ea.getX(), ea.getY());
                                    activeDragger->handle(pointerInfo, ea, aa);
                                    activeDragger = NULL;
                                }
                                return false;
                            case GUIEventAdapter::DRAG:
                                if (activeDragger) {
                                    pointerInfo._hitIter = pointerInfo._hitList.begin();
                                    pointerInfo.setCamera(view->getCamera());
                                    pointerInfo.setMousePosition(ea.getX(), ea.getY());
                                    activeDragger->handle(pointerInfo, ea, aa);
                                    return true;
                                }
                                break;
                            default:
                                break;
                                
                        } 
                    }
                    return false;
                }
                
            private:
                PointerInfo pointerInfo;
                Dragger* activeDragger;
                MeshFile *_mf;
                Local_WGS84_TM_Projection *projWGS84;

            };
            
        }
    }
}



#endif // __POSITIONHANDLER_H

