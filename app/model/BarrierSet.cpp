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

#include "BarrierSet.h"
#include "SimulationState.h"
#include <ctime>
using std::time;
#include <osg/ref_ptr>
using osg::ref_ptr;

namespace ews {
    namespace app {
        namespace model {
            
            /** Ctor. */
            BarrierSet::BarrierSet(SimulationState* parent)  
            : QObject(parent), _barriers() {
                //_potentials = new CompositePotential();
                
                // When barriers are added we need to recompute the PrecomputedPotential.
                //QObject::connect(this, SIGNAL(barrierAdded(int, Barrier*)), this, SLOT(updatePotentials()));
             //   QObject::connect(this, SIGNAL(posChanged(osg::Vec2)), this, SLOT(updatePos(osg::Vec2)));
                
            }
            /** Dtor. */
            BarrierSet::~BarrierSet() {
                removeAllBarriers();
                //if(_potentials) {
                  //  delete _potentials;
                //}
            }
            
            /** Create a new barrier and add it to the list. */
            Barrier* BarrierSet::createBarrier() {        
                static unsigned int count = 1;
                
                // Enforce number limitation.
                if(isFull()) return NULL;

                
                const int pos = _barriers.size();
                //const WaveMedium& water = getSimulationState()->getWaveMedium();
                const unsigned int initX = (int) (count * Barrier::width() * 2) % WIDTH_WATER;//water.getWidth();
                const Vec2 start(initX, 0);
                const Vec2 end(initX, HEIGHT_WATER);//water.getLength());
                
                Barrier* b = new Barrier(this);
                b->setStart(start);
                b->setEnd(end);
                
                // Default name
                b->setObjectName(QString("Barrier %1").arg(count++));
                
                
                // Add to our list of barriers.
                _barriers << b;
                
                // When a barriers parameters are changed we need to update
                // the PrecomputedPotential.
                /*QObject::connect(b, SIGNAL(potentialChanged()), this, SLOT(updatePotentials()));
                
                emit barrierAdded(pos, b);
                emit sizeChanged(size());*/
                return b;
            }
            
            /** Recompute the combined potential grid.
            void BarrierSet::updatePotentials() {
                ref_ptr<CompositePotential> worldPot = new CompositePotential();
                for (QList<Barrier*>::iterator i = _barriers.begin(); i != _barriers.end(); i++) {
                    Barrier* b = *i;
                    if (b->isEnabled()) {
                        worldPot->addPotential(b->getPotential());
                    }
                }
                WaveModel& waveModel = getSimulationState()->getWaveMedium().getWaveModel();
                ref_ptr<PrecomputedPotential> prePot = new PrecomputedPotential(worldPot.get(), waveModel.getWidth(),
                                                                                waveModel.getLength());
                waveModel.setPotential(prePot.get());
            }*/

            /** Delete all barriers. */
            void BarrierSet::removeAllBarriers() {
                for (QList<Barrier*>::iterator i = _barriers.begin(); i != _barriers.end(); i++) {
                    Barrier* b = *i;
                    removeBarrier(b);
                }
            }

            /** Remove the given barrier. */
            void BarrierSet::removeBarrier(Barrier* b) {
                const int pos = indexOf(b);
                if(pos >= 0) {
                    const bool did = _barriers.removeOne(b);
                    if (did) {
                        b->disconnect(this);
                        //updatePotentials();
                        emit barrierRemoved(pos, b);
                        emit sizeChanged(size());
                        delete b;
                    }
                }
            }
            
            /** Convenience method to get the current simulation state parent. */
            SimulationState* BarrierSet::getSimulationState() const {
                QObject* obj = parent();
                return qobject_cast<SimulationState*>(obj);
            }
        }
    }
}


