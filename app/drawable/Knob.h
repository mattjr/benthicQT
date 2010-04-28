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

#ifndef __KNOB_H
#define __KNOB_H

#include <osg/MatrixTransform>
#include <osgManipulator/Dragger>
#include <osgManipulator/Constraint>
#include <osgManipulator/Selection>
namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            /**
             * Provides functionality for picking and dragging screen objects. 
             *
             * The ctor creates a mini-hierarchy with a Dragger and Selection
             * as the immediate children of this, and the selection geometry 
             * (i.e. sphere) as a child of the selection.
             */
            class Knob : public MatrixTransform {
            public:
                /**
                 * Standard ctor.
                 * @param geomOffset translation offset applied to the dragger 
                 * geometry. The selection geometry remains uneffected.
                 * @param radius dragger geometry and hit region radius.
                 * @param showGeom if true, then a sphere is shown as the selection
                 * geometry. If false, the geometry is hidden.
                 */
                Knob(const Vec3& geomOffset = Vec3(0, 0, 0), unsigned int radius = 3, bool showGeom = true);

                /** Get the knob's current location in global coordinates. */
                Vec2 currXYLocation() const {
                    MatrixList transforms = _selectionNode->getWorldMatrices();
                    Matrix toWorld = transforms[0];
                    Vec3 translation = toWorld.getTrans();
                    return Vec2(translation.x(), translation.y());
                }
                void setPosition(const Vec3& pos) {
                    _selectionNode->setMatrix(Matrix::translate(pos));
                    _dragger->setMatrix(Matrix::translate(pos + _geomOffset));
                }
                
                /** Control the dirty flag. */
                void setDirty(bool state) {
                    _dirty = state;
                }
                
                /** Determine if the dirty flag has been set, which is the
                 * case after a drag operation has occurred. It is up to
                 * the client code to reset the dirty flag when appropriate. */
                bool isDirty() const {
                    return _dirty;
                }
                
                /** Set the constraint for the dragger that limits the dragging
                 * to a controlled region.
                 */
                void setConstraint(Constraint& constraint);
                
                
            protected:
                virtual ~Knob();
                
                
            private:
                ref_ptr<Dragger> _dragger;
	        ref_ptr<osgManipulator::Selection> _selectionNode;
                Vec3 _geomOffset;
                bool _dirty;
            };
            
        }
    }
}


#endif // __BARRIER_DRAGGER_H
