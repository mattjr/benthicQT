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

#ifndef __BARRIERSET_H
#define __BARRIERSET_H


#include <QObject>
#include <QList>
#include "Barrier.h"
//#include "CompositePotential.h"

namespace ews {
    namespace app {
        namespace model {
            const int MAX_BARRIERS = 8;
            
            //using ews::physics::CompositePotential;
            
            class SimulationState;
            
            /**
             * Collection of Barrier objects.
             */
            class BarrierSet :  public QObject  {
                Q_OBJECT
            public:
                /**
                 * Default constructor
                 */
                explicit BarrierSet(SimulationState* parent = 0);
                /**
                 * Virtual destructor
                 */
                virtual ~BarrierSet();
                
                /**
                 * Creates a new Barrier and adds it to the collection.
                 * @return Created Barrier
                 */
                Barrier* createBarrier(); 
                
                /**
                 * Gets the SimulationState associated with this collection of Barrier objects.
                 * @return Parent SimulationState.
                 */
                SimulationState* getSimulationState() const;
                
                /**
                 * Gets the number of Barrier objects in this collection.
                 * @return Number of existing Barrier objects (including disabled)
                 */
                int size() const {
                    return _barriers.size();
                }
                
                /**
                 * Determine if we've reached the maximum number of barriers.
                 */
                bool isFull() const {
                    return size() >= MAX_BARRIERS;
                }

                /**
                 * Remove all Barrier objects (e.g., to start over).
                 */
                void removeAllBarriers();

                /**
                 * Remove the given Barrier from this collection.
                 * @param b Barrier to remove
                 */
                void removeBarrier(Barrier* b);
                
                /**
                 * Find the index corresponding to the given Barrier.
                 * @param b Barrier to search for
                 */
                int indexOf(Barrier* b) const {
                    return _barriers.indexOf(b);
                }
                
                /**
                 * Find the Barrier at the given index.
                 * @param index Index to return the Barrier for
                 * @return Requested Barrier
                 */
                Barrier* barrierAt(Uint index) const {
                    return _barriers[index];
                }
                
                /** Reset data to initial conditions. */
                void reset() {
                    removeAllBarriers();
                }
                
            signals:
                /**
                 * Signals that a barrier has been added.
                 */
                void barrierAdded(int,Barrier*);
                /**
                 * Signals that a barrier has been removed.
                 */
                void barrierRemoved(int,Barrier*);
                
                /**
                 * General signal for case when number of items in the
                 * set has changed. Parameter is the new number of items.
                 */
                void sizeChanged(Uint);
                
            private slots:

                
            private:
                Q_DISABLE_COPY(BarrierSet)
                QList<Barrier*> _barriers;

                
            };
        }
    }
}


#endif // __BARRIERSET_H
