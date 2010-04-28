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

#ifndef __SAMPLERSET_H
#define __SAMPLERSET_H

#include <QObject>
#include <QList>
#include "PointSampler.h"

namespace ews {
    namespace app {
        namespace model {
            const int MAX_SAMPLERS = 5;
            
            class SimulationState;
            
            /**
             * Collection of PointSampler objects.
             */
            class SamplerSet :  public QObject  {
                Q_OBJECT
            public:
                /**
                 * Default constructor
                 */
                explicit SamplerSet(SimulationState* parent);
                /**
                 * Virtual destructor
                 */
                virtual ~SamplerSet();

                
                /** 
                 * Create instance which samples and maintains a history
                 * of values at a  set x/y location. This takes
                 * ownership of memory.
                 * @return Created PointSampler
                 */
                PointSampler* createSampler(); 

                /**
                 * Gets the number of PointSampler objects in this collection.
                 * @return Number of existing PointSampler objects (including disabled)
                 */
                int size() const {
                    return _samplers.size();
                }
                
                /** 
                 * Determine if the set has reached the maximum allowable
                 * samplers.
                 */
                bool isFull() const {
                    return _samplers.size()  >= MAX_SAMPLERS;
                }

                /**
                 * Remove all PointSampler objects (e.g., to start over).
                 */
                void removeAllSamplers();

                /**
                 * Remove the given PointSampler from this collection.
                 * @param b PointSampler to remove
                 */
                void removeSampler(PointSampler* b);
                
                /**
                 * Find the index corresponding to the given PointSampler.
                 * @param b PointSampler to search for
                 */
                int indexOf(PointSampler* b) const {
                    return _samplers.indexOf(b);
                }
                
                /**
                 * Find the PointSampler at the given index.
                 * @param index Index to return the PointSampler for
                 * @return Requested PointSampler
                 */
                PointSampler* samplerAt(Uint index) const {
                    return _samplers[index];
                }
                
                /**
                 * Gets the SimulationState associated with this collection of PointSampler objects.
                 * @return Parent SimulationState.
                 */
                SimulationState* getSimulationState() const;

            public slots:
                /** Pause sampling. */
                void setPaused(bool state);
                /** Reset data to initial conditions. */
                void reset();
                
            signals:
                /**
                 * Signals that a sampler has been added.
                 */
                void samplerAdded(int,PointSampler*);
                /**
                 * Signals that a sampler has been removed.
                 */
                void samplerRemoved(int,PointSampler*);
                
            private:
                Q_DISABLE_COPY(SamplerSet)
                typedef QList<PointSampler*> PointSamplerList;
                typedef QList<PointSampler*>::iterator PointSamplerIterator;
                PointSamplerList _samplers;
            };
        }
    }
}


#endif // __SAMPLERSET_H
