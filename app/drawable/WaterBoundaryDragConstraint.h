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
#ifndef __WATER_BOUNDARY_DRAG_CONSTRAINT_H
#define __WATER_BOUNDARY_DRAG_CONSTRAINT_H


#include <osgManipulator/CommandManager>
#include <osgManipulator/Constraint>
#include <osgManipulator/Command>
#include <osg/BoundingBox>

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;

            /**
             * Constraint class used to prevent Barrier and DripSource objects from being moved
             * out-of-bounds.
             */
            class WaterBoundaryDragConstraint : public Constraint {
            public:
                
                WaterBoundaryDragConstraint(Node& refNode, const osg::BoundingBox& bounds)
                : Constraint(refNode), _bounds(bounds)  {
                }
                
                inline Vec3d clampPoint(Vec3 p) const {
                    Vec3 retval;
                    retval.x() = osg::clampTo(p.x(), _bounds.xMin(), _bounds.xMax());
                    retval.y() = osg::clampTo(p.y(), _bounds.yMin(), _bounds.yMax());
                    retval.z() = p.z();
                    return retval;
                }
                
                virtual bool constrain(TranslateInPlaneCommand& command) const {
                    
                    if (command.getStage() == MotionCommand::START)
                        computeLocalToWorldAndWorldToLocal();
                    else if (command.getStage() == MotionCommand::FINISH)
                        return true;
                    
                    Vec3 translation = command.getReferencePoint() + command.getTranslation();
                    Vec3 destination = translation * command.getLocalToWorld();
                    destination = clampPoint(destination);
                    command.setTranslation(destination * command.getWorldToLocal());

                    return true;
                }
                
            protected:
                virtual ~WaterBoundaryDragConstraint() {}
                
            private:
                osg::BoundingBox _bounds;
                
            };
        }
    }
}




#endif // __WATER_BOUNDARY_DRAG_CONSTRAINT_H

