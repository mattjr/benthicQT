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

#include "Lattice.h"
#include <algorithm>
using std::max;
using std::min;

namespace ews {
    namespace physics {
        /** 
         * Data type for storing indexes into amplitude data. profiler shows that unsigned short is /slightly/
         * faster than unsigned int. Overflow shouldn't be a concern in the near term.
         */
        typedef unsigned short LoopType;
        LatticeVal Lattice::computeAverageValue(unsigned int x, unsigned int y, unsigned int windowWidth) const {
            LatticeVal sum = 0.0;
            if (getSize() == 0) return sum;
            const LoopType minX = max(static_cast<int>(x) - static_cast<int>(windowWidth), 0);
            const LoopType maxX = min(x + windowWidth, static_cast<unsigned int>(getWidth()) - 1);
            const LoopType minY = max(static_cast<int>(y) - static_cast<int>(windowWidth), 0);
            const LoopType maxY = min(y + windowWidth, static_cast<unsigned int>(getLength()) - 1);
            const LoopType count = (maxX - minX + 1) * (maxY - minY + 1);
            
            const LoopType len = getLength();
            
            LoopType i, j;
            for (i = minX; i <= maxX; i++) {
                const LoopType rowOffset = i * len;
                for (j = minY; j <= maxY; j++) {
                    sum += _amplitudeData[rowOffset + j];
// same as:         sum += getValue(i, j);
                }
            }
            return sum / count;    
        }
        void Lattice::scale(LatticeVal scaleVal) {
            if (scaleVal == 1.0) return;

            unsigned int size = getSize();
            for(unsigned int i = 0; i < size; i++) {
                _amplitudeData[i] *= scaleVal;
            }
        }
    }
}
