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


/* This file modified version of osgGA::TerrainManipulator. Not enough of the
 * implementation was virtual to get desired behavior from subclassing. */
#ifndef __CAMERA_CONTROLLER_H
#define __CAMERA_CONTROLLER_H

#include <osgGA/MatrixManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/GUIEventAdapter>

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgGA;
            const double MIN_ZOOM_DISTANCE = 75;
            const double MAX_ZOOM_DISTANCE = 500;
            
            /**
             * Hack of osgGA::TerrainManipulator for
             * setting up some defaults and limiting movement to some degree.
             */
            class CameraController :  public osgGA::TerrainManipulator  {
            public:
                CameraController();
                virtual void computeHomePosition();

                /** handle events, return true if handled, false otherwise.*/
                virtual bool handle(const GUIEventAdapter& ea, GUIActionAdapter& us);
                
            protected:
                
                virtual ~CameraController();
                
                /** Apply rotation based on mouse change in x and y. */
                virtual void doRotate(double dx, double dy);
                /** Apply "zoom" based on mouse change in x and y. */
                virtual void doZoom(double dx, double dy);
                /** Apply "pan" based on mouse change in x and y. */
                virtual void doPan(double dx, double dy);
                
                void computePosition(const Vec3d& eye,const Vec3d& lv,const Vec3d& up);
                
                /** For the give mouse movement calculate the movement of the camera.
                 Return true is camera has moved and a redraw is required.*/
                virtual bool calcMovement();
                /**
                 * Prevents certain kinds of rotations
                 */
                virtual void clampOrientation();
            };
            
        }
    }
}


#endif //__CAMERA_CONTROLLER_H

