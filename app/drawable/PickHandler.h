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
 * Modified by Matthew Johnson-Roberson 
 * from original source: Visualize Physics - Wave 
 * Copyright (C) 2012 Matthew Johnson-Roberson
 * See COPYING for license details
 */

#ifndef __PICKHANDLER_H
#define __PICKHANDLER_H

#include <osgGA/GUIEventHandler>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgManipulator/Dragger>
#include <osgUtil/LineSegmentIntersector>
#include <osgViewer/View>
#include "BQTDebug.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgGA;
            using namespace osgUtil;
            using namespace osgManipulator;
            typedef LineSegmentIntersector::Intersections::iterator IntersectIterator;
            
            /**
             * Class responsible for determining which object in a scene graph has been clicked on.
             */
            class PickHandler :  public osgGA::GUIEventHandler  {
                
            public:
                
                PickHandler() : GUIEventHandler(), activeDragger(NULL) { }                                                       
                
                /** Handle GUI event. */
                virtual bool handle(const GUIEventAdapter& ea,
                                    GUIActionAdapter& aa, osg::Object* obj, NodeVisitor* nv) { 

                    osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
                    if (view) {
                        
                        switch (ea.getEventType()) {
                                
                            case GUIEventAdapter::PUSH: 
                            {
                                LineSegmentIntersector::Intersections intersections;
                                pointerInfo.reset();
                                
                                if (view->computeIntersections(ea.getX(), ea.getY(), intersections)) {
                                    
                                    pointerInfo.setCamera(view->getCamera());
                                    pointerInfo.setMousePosition(ea.getX(), ea.getY());
                                    
                                    // qDebug() << "PointerInfo:" << pointerInfo;
                                    
                                    for (IntersectIterator iter = intersections.begin(); 
                                         iter != intersections.end(); 
                                         ++iter) {
                                        pointerInfo.addIntersection(iter->nodePath, iter->getLocalIntersectPoint());
                                    }
                                    
                                    for (NodePath::iterator iter = pointerInfo._hitList.front().first.begin(); 
                                         iter != pointerInfo._hitList.front().first.end(); 
                                         ++iter) {	
                                        
                                        // qDebug() << "hit" << (*iter)->getName();
                                        if (Dragger* dragger = dynamic_cast<Dragger*>(*iter)) {
                                            dragger->handle(pointerInfo, ea, aa);
                                            activeDragger = dragger;
                                            qDebug() << "Active dragger is:" << activeDragger->className() << activeDragger->getName().c_str();
                                            return false;
                                        }
                                    }
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

            };
            
        }
    }
}



#endif // __PICKHANDLER_H

