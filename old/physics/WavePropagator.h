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

#ifndef __WAVEPROPAGATOR_H
#define __WAVEPROPAGATOR_H

#include "Potential.h"
#include "Lattice.h"
#include <osg/Referenced>
#include <osg/ref_ptr>

namespace ews {
    namespace physics {
        using osg::ref_ptr;
        
        /**
         * @ingroup Physics
         * Interface for models describing wave propagation.
         */        
        class WavePropagator :  public osg::Referenced  {
        public:
            /**
             * Virtual destructor
             */
            virtual ~WavePropagator() { /* do nothing */ }
            /**
             * Adjusts the given point as if it had an immediate history of having the given value there.
             * @param x X-location to modify
             * @param y Y-location to modify
             * @param value Value to assign
             */            
            virtual void setBoundaryCondition(unsigned int x, unsigned int y, LatticeVal value) = 0;
            /**
             * Update the given lattice using history stored in this propagator.
             * @param lattice Lattice to update
             */
            virtual void propagate(Lattice& lattice) = 0;
            /**
             * Scale the histories of this propagator by the given factor.
             * @param scale Amount to scale by
             */
            virtual void scale(LatticeVal scale) = 0;
            /**
             * Adjusts the region over which this propagator is valid.
             */
            virtual void setSize(unsigned int width, unsigned int length) = 0;
            /**
             * Returns the potential associated with this propagator at location x, y.
             * @param x
             * @param y
             * @return Associated potential
             */
            virtual Real getPotential(Uint x, Uint y) const = 0;
            /**
             * Returns the potential associated with this propagator.
             * @return Associated potential
             */
            virtual const ref_ptr<Potential> getPotential() const = 0;
            /**
             * Assigns a new potential to this propagator.
             * @param p New potential
             */
            virtual void setPotential(Potential* p) = 0;
            /**
             * Resets the immediate history to zero.
             */
            virtual void clear() = 0;
        protected:
            WavePropagator() { /* do nothing */ }
        private:
            WavePropagator(const WavePropagator&) {} // Not allowed
            WavePropagator& operator=(const WavePropagator& l) { return *this; } // Not allowed
        };
    }
}

#endif // __WAVEPROPAGATOR_H
