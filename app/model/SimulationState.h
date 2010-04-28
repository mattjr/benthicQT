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

#ifndef SimulationState_H
#define SimulationState_H

#include <QtCore>
#include <QList>

#include "MeshFile.h"
#include "BarrierSet.h"
#include "Barrier.h"
namespace ews {
    namespace app {
        namespace model {
            
            /** This class is the root of the simulation state; all transient data
             *  should be available from this class and/or its children.
             */
            class SimulationState :  public QObject  {
                Q_OBJECT
                
            public:
                explicit SimulationState(QObject * parent = 0);
                virtual ~SimulationState();
               

                /**
                 * Get the set of defined barriers.
                 */
                BarrierSet& getBarriers() {
                    return _barriers;
                }
                


                
                /**
                 * Determine if the simulation is currently in the "paused" state.
                 */
                bool isPaused() const {
                   return false; //return _waveMedium.isPaused();
                }
                
                
            signals:
                /** 
                 * Signal fired when a wall or barrier is added to the scene.
                 */
                void objectAdded(QObject& added);
                /**
                 * Signal fired when a wall or barrier is removed from the scene.
                 */
                void objectRemoved(QObject& removed);
                
            public:
                /**
                 * This is a convenience method for the setup phase when
                 * signals are getting registered and we want to 
                 * associate geometry with the default simulation state
                 * objects. It should only be called by once by the setup code.
                 */
                void emitSignalsForDefaults() {
                  /*  emit objectAdded(getWaveMedium());
                    emit objectAdded(getDripSource1());
                    emit objectAdded(getDripSource2());*/
                }
                   void emitSignalsForLoad() {
                    emit objectAdded(getMeshFiles());

                }
                   MeshFile &getMeshFiles(void){
                        return _mesh_files;
                    }
                
            public slots:
                /** Pause the simulation, whereby the wave propagation is stopped. */
                void setPaused(bool);
                /** Reset the simulation and view to the default view. */
                void reset();

                void forwardMeshLoad(MeshFile* meshfile);
                /** Due to an "impedence mismatch" between the signal fired
                 * by the BarrierSet class and our objectAdded() signal, 
                 * a conversion/mapping slot is is used to fire objectAdded()
                 * with the proper arguments. 
                 */
                void forwardBarrierSetAddition(int index, Barrier* barrier);
                /** See forwardBarrierSetAddition(int, Barrier*) */
                void forwardBarrierSetRemoval(int index, Barrier* barrier);
                /** See forwardBarrierSetAddition(int, Barrier*) */




            private:
                Q_DISABLE_COPY(SimulationState)          
                BarrierSet _barriers;
                MeshFile _mesh_files;

            };
        }
    }
}
#endif // SimulationState_H
