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

#ifndef __SLITPOTENTIAL_H
#define __SLITPOTENTIAL_H

#include "WallPotential.h"
#include <osg/Vec2>
#include <vector>
using std::vector;
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        
        using osg::Vec2;
        
        const Real DEFAULT_SLIT_WIDTH = 3.f;
        const Real DEFAULT_WALL_THICKNESS = 4.f;
        
        /**
         * @ingroup Physics
         * Potential with a constant value everywhere.
         */        
        class SlitPotential : public WallPotential {
        public:
            /**
             * Constructor taking number of slits
             * @param numSlits how many slits to create
             */
            SlitPotential(const Vec2& p1, const Vec2& p2, unsigned int numSlits = 0);
            /**
             * Virtual destructor
             */
            virtual ~SlitPotential() { /* do nothing */}
            /**
             * Add a slit at a specified fraction of the distance along its wall
             * @param alphaVal fraction of the distance along the wall to add the slit
             */
            void addSlit(Real alphaVal);
            /**
             * Remove the nth slit
             * @param slitNumber Which slit to delete (slits are sorted by location)
             */
            void removeSlit(unsigned int slitNumber) {
                if (slitNumber < _slitAlphas.size()) {
                    _slitAlphas.erase(_slitAlphas.begin() + slitNumber);
                }
            }
            
            Vec2 getSlitLocation(unsigned int slitNumber) const;
            
            /**
             * Get the width of the slits
             */
            Real getSlitWidth() const { return _slitWidth; }
            /**
             * Set the width of the slits
             * @param slitWidth New slit width (values less than zero will use zero instead)
             */
            void setSlitWidth(Real slitWidth) { _slitWidth = slitWidth > 0 ? slitWidth : 0.0; }
            /**
             * Returns the potential at x, y
             * @param x X location to check
             * @param y Y location to check
             * @return Potential
             */
            virtual Real getPotential(unsigned int x, unsigned int y) const;
        private:
            SlitPotential(const SlitPotential&) {} // Not allowed
            SlitPotential& operator=(const SlitPotential& l) { return *this; } // Not allowed
            Real _slitWidth;
            // Fraction that each slit is along the wall
            vector<Real> _slitAlphas;
        };
    }
}

#endif // __SLITPOTENTIAL_H
