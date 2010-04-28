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

#ifndef __POTENTIAL_UPDATER_H
#define __POTENTIAL_UPDATER_H

#include <osg/NodeCallback>
#include "BarrierGeom.h"
#include "BQTDebug.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            /**
             * Class responsible for updating the potential data %model when BarrierGeom changes.
             */
            class PotentialUpdater : public NodeCallback {
                
            public:
                
                /** Callback method. */
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    BarrierGeom* geom = dynamic_cast<BarrierGeom*>(node);
                    if(geom) {
                        geom->checkKnobs();
                    }
                    else {
                        notify(osg::WARN) << "PotentialUpdater didn't receieve the right geometry type";
                        return;
                    }
                    
                    traverse(node,nv);
                }
            };
        }
    }
}


#endif // __POTENTIAL_UPDATER_H

