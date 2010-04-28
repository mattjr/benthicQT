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

#ifndef __EWSDEFINE_H
#define __EWSDEFINE_H
#define WIDTH_WATER 10
#define HEIGHT_WATER 10
namespace ews {
    /**
     * Default precision to use for real numbers.
     */
    typedef float Real;
    /**
     * Unsigned integer. Not using UInt as that might conflict with other definitions.
     */
    typedef unsigned int Uint;
    /**
     * Flag to enable more realistic water dripping wave. Specifically, if this flag is turned
     * on, the wavelength in water isn't connected to the frequency of the oscillator.
     */
    const bool REALISTIC_DRIP = false;
}

#endif // __EWSDEFINE_H
