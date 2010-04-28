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

#include "PrecomputedPotential.h"

namespace ews {
    namespace physics {
        PrecomputedPotential::PrecomputedPotential(Potential* p, unsigned int width,
                                                   unsigned int length):
        _potentialValues(width, vector<Real>(length, 0.0)), _width(width), _length(length) {
            for (unsigned int i = 0; i < width; i++) {
                for (unsigned int j = 0; j < length; j++) {
                    _potentialValues[i][j] = p->getPotential(i, j);
                }
            }
        }
    }
}
