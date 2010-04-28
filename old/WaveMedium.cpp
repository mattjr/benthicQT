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


#include "WaveMedium.h"

namespace ews {
    namespace app {
        namespace model {

            void WaveMedium::updateWaveModel() {
                using ews::physics::WaveModel;

                
                if(_waveModel) {
                    delete _waveModel;
                    _waveModel = 0;
                }
                
                
                _waveModel = new WaveModel(_width * _latticeDivisionsPerCentimeter, 
                                           _length * _latticeDivisionsPerCentimeter);
            }
            
        }
    }
}
