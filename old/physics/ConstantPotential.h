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

#ifndef __CONSTANTPOTENTIAL_H
#define __CONSTANTPOTENTIAL_H

#include "Potential.h"
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        
        /**
         * @ingroup Physics
         * Potential with a constant value everywhere.
         */        
        class ConstantPotential : public Potential {
        public:
            /**
             * Default constructor, assigning a value of 0
             */
            explicit ConstantPotential(): _value(0.0) { /* do nothing */ }
            /**
             * Constructor, assigning value to the constant potential everywhere. Explicit to prevent
             * automatic conversions from reals to ConstantPotentials.
             * @param value Constant potential
             */
            explicit ConstantPotential(Real value): _value(value) { /* do nothing */ }
            /**
             * Virtual destructor
             */
            virtual ~ConstantPotential() { /* do nothing */}
            /**
             * Returns the constant potential, regardless of x or y
             * @param x ignored
             * @param y ignored
             * @return Constant potential
             */
            virtual Real getPotential(unsigned int x, unsigned int y) const {
                return _value;
            }
        private:
            ConstantPotential(const ConstantPotential&) {} // Not allowed
            ConstantPotential& operator=(const ConstantPotential& l) { return *this; } // Not allowed
            Real _value;
        };
    }
}

#endif // __CONSTANTPOTENTIAL_H
