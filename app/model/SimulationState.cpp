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

#include "SimulationState.h"
#include <osg/Vec2>

namespace ews {
    namespace app {
        namespace model {
            
            SimulationState::SimulationState(QObject * parent) 
            : QObject(parent),
            _barriers(this) {
                // We need to forward/create objectAdded events
                // from the BarrierSet.
                
                connect(&_barriers, SIGNAL(barrierAdded(int,Barrier*)),
                        SLOT(forwardBarrierSetAddition(int, Barrier*)));
                connect(&_barriers, SIGNAL(barrierRemoved(int,Barrier*)),
                        SLOT(forwardBarrierSetRemoval(int, Barrier*)));
               
              //  connect(&_mesh_files, SIGNAL(updatePos(osg::Vec3)),
                     //   SLOT(forwardPos(osg::Vec3)));


                setPaused(true);
            }
            
            SimulationState::~SimulationState()  {
            }
            
            void SimulationState::setPaused(bool state) {

            }
                                            
            void SimulationState::forwardBarrierSetAddition(int index, Barrier* barrier) {
                emit objectAdded(*barrier);
            }
            
            void SimulationState::forwardBarrierSetRemoval(int index, Barrier* barrier) {
                emit objectRemoved(*barrier);
            }
            
            void SimulationState::forwardMeshLoad(MeshFile *mesh) {
                emit objectAdded(*mesh);
            }
            
            void SimulationState::reset() {

                _barriers.reset();

                // Hack to make sure cleared wave model gets rendered.
                if (isPaused()) {
//                    _waveMedium.getWaveModel().propagate();
                }
            }


        }
    }
}
