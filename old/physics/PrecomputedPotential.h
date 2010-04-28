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

#ifndef __PRECOMUPTEDPOTENTIAL_H
#define __PRECOMUPTEDPOTENTIAL_H

#include "Potential.h"
#include <vector>
using std::vector;
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        
        /**
         * @ingroup Physics
         * Potential that pre-calculates the values of another potential for all valid spatial locations.
         */        
        class PrecomputedPotential : public Potential {
        public:
            /**
             * Constructor that actually calculates a potential
             * @param p Potential to precompute
             * @param width Width of region where potential is valid (region is valid from 0 to width - 1)
             * @param length Length of region where potential is valid (region is valid from 0 to length - 1)
             */
            PrecomputedPotential(Potential* p, unsigned int width, unsigned int length);
            /**
             * Virtual destructor
             */
            virtual ~PrecomputedPotential() { /* do nothing */}
            /**
             * Returns the precomputed potential for location (x, y)
             * @param x X location to find potential for
             * @param y Y location to find potential for
             * @return Precomputed potential
             */
            Real getPotential(unsigned int x, unsigned int y) const {
                return (x < getWidth()) && (y < getLength()) ? _potentialValues[x][y] : 0.0;
            }
        private:
            PrecomputedPotential(): _potentialValues(), _width(0), _length(0) {} // Not allowed
            PrecomputedPotential(const PrecomputedPotential&) {} // Not allowed
            PrecomputedPotential& operator=(const PrecomputedPotential& l) { return *this; } // Not allowed
            inline unsigned int getWidth() const { return _width; }
            inline unsigned int getLength() const { return _length; }
            vector<vector<Real> > _potentialValues;
            unsigned int _width;
            unsigned int _length;
        };
    }
}

#endif // __PRECOMUPTEDPOTENTIAL_H
