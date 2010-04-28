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

#ifndef __DAMPEDCLASSICALWAVEPROPAGATOR_H
#define __DAMPEDCLASSICALWAVEPROPAGATOR_H

#include "WavePropagator.h"
#include "Potential.h"
#include "Lattice.h"
#include <osg/ref_ptr>
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        using osg::ref_ptr;
        const Uint DEFAULT_DAMP_LENGTH = 50;
        const LatticeVal NEIGHBOR_PROPAGATE = 0.14f;
        const LatticeVal DIAG_PROPAGATE = 0.06f;
        const LatticeVal SELF_PROPAGATE = 1.1f;
        const LatticeVal MEMORY_TERM = 0.95f;
        // How much energy would go into the next timestep without additional adjustment
        const LatticeVal XFR_FACTOR = 4 * (NEIGHBOR_PROPAGATE + DIAG_PROPAGATE) + SELF_PROPAGATE - MEMORY_TERM;
        // How much energy we want to be "lost" (0.99 means 1% loss)
        const LatticeVal DAMP_FACTOR = 0.99;
        const LatticeVal MULT_FACTOR = DAMP_FACTOR / XFR_FACTOR;
        
        /**
         * @ingroup Physics
         * Wave Propagator using a damped classical model.
         */        
        class DampedClassicalWavePropagator : public WavePropagator {
        public:
            /**
             * Constructor that creates a  wave propagator for a region of size width x length, with potential p, and
             * with x/y dampening factors of dampX/dampY
             * @param p Potential associated with this propagator
             * @param width Width of the region associate with this propagator
             * @param length Length of the region associate with this propagator
             * @param dampX Dampening factor in the x direction
             * @param dampY Dampening factor in the y direction
             */
            DampedClassicalWavePropagator(Potential* p, Uint width, Uint length, Uint dampX, Uint dampY);
            /**
             * Virtual destructor
             */
            virtual ~DampedClassicalWavePropagator() { /* do nothing */ }
            /**
             * Adjusts the given point as if it had an immediate history of having the given value there.
             * @param x X-location to modify
             * @param y Y-location to modify
             * @param value Value to assign
             */
            void setBoundaryCondition(Uint x, Uint y, LatticeVal value);
            /**
             * Update the given lattice using history stored in this propagator.
             * @param lattice Lattice to update
             */
            void propagate(Lattice& lattice);
            /**
             * Scale the histories of this propagator by the given factor.
             * @param scale Amount to scale by
             */
            void scale(LatticeVal scale);
            /**
             * Adjusts the region over which this propagator is valid. If the valid differs from the existing
             * size, it resets all history to zero.
             */
            void setSize(Uint width, Uint length);
            /**
             * Returns the potential associated with this propagator at location x, y.
             * @param x
             * @param y
             * @return Associated potential
             */
            Real getPotential(Uint x, Uint y) const {
                return _potential->getPotential(x + _dampX, y + _dampY);
            }
            /**
             * Returns the potential associated with this propagator.
             * @return Associated potential
             */
            const ref_ptr<Potential> getPotential() const { return _potential; }
            /**
             * Assigns a new potential to this propagator.
             * @param p New potential
             */
            void setPotential(Potential* p);
            /**
             * Resets the immediate history to zero.
             */
            void clear();
        private:
            DampedClassicalWavePropagator(const DampedClassicalWavePropagator&) {} // Not allowed
            DampedClassicalWavePropagator& operator=(const DampedClassicalWavePropagator& l) { return *this; } // Not allowed
            /**
             * Subtracts the dampening offset added by DampedClassicalWavePropagator
             */
            class PaddedPotential : public Potential {
            public:
                PaddedPotential(Potential* p, Uint dampX, Uint dampY)
                : _innerP(p), _dampX(dampX), _dampY(dampY) {
                    /* do nothing */
                }
                virtual ~PaddedPotential() { /* do nothing */ }
                Real getPotential(Uint x, Uint y) const {
                    return _innerP->getPotential(x - _dampX, y - _dampY);
                }
            private:
                ref_ptr<Potential> _innerP;
                Uint _dampX;
                Uint _dampY;
            };
            void dampHorizontal(Uint y, int delta);
            void dampHorizontal(Uint y, int delta, Uint numDampPts);
            void dampVertical(Uint x, int delta);
            void dampVertical(Uint x, int delta, Uint numDampPts);
            void dampScale();
            LatticeVal getDamp(Uint depthInDampRegion);
            ref_ptr<Potential> _potential;
            Lattice _largeLattice;
            Lattice _priorLattice;
            Lattice _priorPriorLattice;
            Uint _dampX;
            Uint _dampY;
        };
    }
}

#endif // __DAMPEDCLASSICALWAVEPROPAGATOR_H
