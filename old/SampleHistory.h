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

#ifndef __SAMPLE_HISTORY_H
#define __SAMPLE_HISTORY_H

#include "EWSDefine.h"
#include "Lattice.h"
#include <deque>
#include <algorithm>

namespace ews {
    namespace app {
        namespace model {
            using ews::Uint;
            using ews::physics::LatticeVal;
            
            /** Class responsible for (efficiently) maintaining a rolling history of amplitude values. */
            class SampleHistory {
            public:
                SampleHistory(Uint historySize)
                : _history(historySize, 0), _size(historySize) {}
                
                void addValue(LatticeVal v) {
                    // Update history.
                    _history.pop_front();
                    _history.push_back(v);
                }
                
                inline Uint size() const {
                    return _size;
                }
                
                inline LatticeVal valueAt(Uint index) const {
                    return _history[index];
                }
                
                void clear() {
                    _history.assign(_size, 0);
                }
                
            private:
                std::deque<LatticeVal> _history;
                const Uint _size;
            };
        }
    }
}




#endif // __SAMPLE_HISTORY_H
