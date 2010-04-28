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

#ifndef __POTENTIAL_H
#define __POTENTIAL_H

#include <osg/Referenced>
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        /**
         * @ingroup Physics
         * Stores field potentials.
         */
        class Potential :  public osg::Referenced  {
        public:
            
            /**
             * Returns the potential at location x, y
             * @param x X location to find potential for
             * @param y Y location to find potential for
             * @return Potential
             */
            virtual Real getPotential(unsigned int x, unsigned int y) const = 0;
//        protected:
            Potential() { /* do nothing */ }
            /**
             * Virtual destructor. Protected for intrusive pointer reference counting.
             */
            virtual ~Potential() { /* do nothing */ }
            
        private:
            Potential(const Potential&) {} // Not allowed
            Potential& operator=(const Potential& l) { return *this; } // Not allowed
        };
    }
}

#endif // __POTENTIAL_H
