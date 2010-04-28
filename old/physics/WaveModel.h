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

#ifndef __WAVEMODEL_H
#define __WAVEMODEL_H

#include "Lattice.h"
#include "WavePropagator.h"
#include <osg/ref_ptr>
using osg::ref_ptr;

/**
 * Parent namespace for all code specific to NVIDIA Foundation simulations
 */
namespace ews {
    /**
     * Contains physics and math classes independent of UI
     */
    namespace physics {
        /**
         * @ingroup Physics
         * Model for waves using the DampedClassicalWavePropagator.
         */        
        class WaveModel {
        public:
            /**
             * Constructor assigning a valid region to a wave model.
             * @param width Width of the wave model's valid field.
             * @param length Length of the wave model's valid field.
             */
            WaveModel(unsigned int width, unsigned int length);
            /**
             * Constructor assigning dampening factors and a valid region to a wave model.
             * @param width Width of the wave model's valid field.
             * @param length Length of the wave model's valid field.
             * @param dampX Dampening in the x-direction
             * @param dampY Dampening in the y-direction
             */
            WaveModel(unsigned int width, unsigned int length, unsigned int dampX, unsigned int dampY);
            /**
             * Virtual destructor
             */
            virtual ~WaveModel() { /* do nothing */ }
            /**
             * Propagate the wave one "time step"
             */
            void propagate() { _wavePropagator->propagate(_lattice); }
            /**
             * Change the valid region of the wave model.
             */
            void setSize(unsigned int width, unsigned int length) {
                _lattice.setSize(width, length);
                _wavePropagator->setSize(width, length);
            }
            /**
             * Determine whether the wave model is valid for the specified location.
             * @param x X-location to check
             * @param y Y-location to check
             * @return Whether x,y is valid for this wave model.
             */
            bool containsLocation(unsigned int x, unsigned int y) const {
                return _lattice.containsLocation(x, y);
            }
            /**
             * Get the current wave amplitude at the specified location.
             * @param x X-location to check
             * @param y Y-location to check
             * @return Wave amplitude
             */
            LatticeVal getValue(unsigned int x, unsigned int y) const { return _lattice.getValue(x, y); }
            /**
             * Compute the average wave amplitude over a region of the model.
             * @param x X-location to check
             * @param y Y-location to check
             * @param windowWidth The window size over which to average.
             */
            LatticeVal computeAverageValue(unsigned int x, unsigned int y, unsigned int windowWidth) const {
                return _lattice.computeAverageValue(x, y, windowWidth);
            }
            /**
             * Set the current and historical values of a location to a specific value.
             * @param x X-location to set
             * @param y Y-location to set
             * @param value New value for the location
             */
            void setSourceValue(unsigned int x, unsigned int y, LatticeVal value) {
                _lattice.setValue(x, y, value);
                _wavePropagator->setBoundaryCondition(x, y, value);
            }
            /**
             * Get the width of the valid region for this wave model.
             * @return Valid region's width (corresponds to x)
             */
            unsigned int getWidth() const { return _lattice.getWidth(); }
            /**
             * Get the length of the valid region for this wave model.
             * @return Valid region's length (corresponds to y)
             */
            unsigned int getLength() const { return _lattice.getLength(); }
            /**
             * Get the potential associated with this wave model.             
             */
            const ref_ptr<Potential> getPotential() const {
                return _wavePropagator->getPotential();
            }
            /**
             * Get the potential associated with this wave model at location (x,y)
             * @param x
             * @param y
             */
            Real getPotential(Uint x, Uint y) const {
                return _wavePropagator->getPotential(x, y);
            }
            /**
             * Assign a new potential to be used by this wave model.
             */
            void setPotential(Potential* p) { _wavePropagator->setPotential(p); }
            
            /**
             * Get direct access to the low level storage of the amplitude
             * data.
             * @return vector of vectors containing amplitude values.
             */
            const Lattice& getLattice() const {
                return _lattice;
            }
            
            /**
             * Reset the state of the wave model.
             */
            void clear() {
                _lattice.clear();
                _wavePropagator->clear();

            }
        private:
            WaveModel(const WaveModel&) {} // Not allowed
            WaveModel& operator=(const WaveModel& l) { return *this; } // Not allowed
            static WavePropagator* createDefaultPropagator(unsigned int width, unsigned int length,
                                                           unsigned int dampX, unsigned int dampY);
            Lattice _lattice;
            ref_ptr<WavePropagator> _wavePropagator;
        };
    }
}


#endif // __WAVEMODEL_H
