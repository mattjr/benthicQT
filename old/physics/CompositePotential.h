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

#ifndef __COMPOSITEPOTENTIAL_H
#define __COMPOSITEPOTENTIAL_H

#include "Potential.h"
#include <vector>
#include <osg/ref_ptr>
using std::vector;
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        using osg::ref_ptr;
        typedef vector<ref_ptr<Potential> > PotentialList;
        
        /**
         * @ingroup Physics
         * Potential made up of the sum of other potentials.
         */        
        class CompositePotential : public Potential {
        public:
            /**
             * Default constructor (has no potentials)
             */
            CompositePotential(): Potential() { /* do nothing */ }
            /**
             * Virtual destructor
             */
            virtual ~CompositePotential() { /* do nothing */ }
            /**
             * Get potential from sum of the composite's potentials.
             * @param x X location to find potential for
             * @param y Y location to find potential for
             * @return Potential
             */
            Real getPotential(unsigned int x, unsigned int y) const;
            /**
             * Add a potential to this composite list of potentials.
             * @param p Potential to add
             */
            void addPotential(Potential* p);
            /**
             * Remove a potential from this composite list of potentials. If potential
             * is not in current list of potentials, nothing happens.
             * @param p Potential to remove
             */
            void removePotential(Potential* p);
        private:
            CompositePotential(const CompositePotential&) {} // Not allowed
            CompositePotential& operator=(const CompositePotential& l) { return *this; } // Not allowed
            PotentialList _potentials;
        };
    }
}

#endif // __COMPOSITEPOTENTIAL_H
