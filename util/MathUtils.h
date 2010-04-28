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

#ifndef __MATHUTILS_H
#define __MATHUTILS_H

#include "BQTDefine.h"
#include <algorithm>

namespace ews {
    namespace util {
        const Real MAX_PERCENT = 100;

        /**
         * Return x if x is between min and max (inclusive), else return min if x is less than min
         * or max if x is greater than max
         * @param lower Minimum allowable value
         * @param x Value to impose bounds on
         * @param upper Maximum allowable value
         * @return Bounded x
         */
        inline Real imposeBounds(Real lower, Real x, Real upper) {
            using std::max;
            using std::min;
            Real r1 = max(lower, x);
            return min(r1, upper);
        }
        /**
         * Return a value frac distance between min and max.
         * @param min Minimum allowable value (if frac is between 0 and 1)
         * @param frac Fractional distance from min to max (usually between 0 and 1)
         * @param max Maximum allowable value (if frac is between 0 and 1)
         * @return Value scaled between min and max
         */
        inline Real scale(Real min, Real frac, Real max) {
            return (frac * (max - min)) + min;
        }
        /**
         * Return a value frac distance between min and max.
         * @param min Minimum allowable value (if frac is between 0 and 1)
         * @param frac Percentage distance from min to max (between 0 and 100)
         * @param max Maximum allowable value (if frac is between 0 and 1)
         * @return Value scaled between min and max
         */
        inline Real scale(Real min, Uint frac, Real max) {
            return scale(min, static_cast<Real>(frac) / MAX_PERCENT, max);
        }
    }
}

#endif //__MATHUTILS_H
