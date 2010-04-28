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

#ifndef __PRECOMPUTEDPOTENTIALTEST_H
#define __PRECOMPUTEDPOTENTIALTEST_H

#include "PrecomputedPotential.h"
using namespace ews::physics;

#include <QtTest/QtTest>

namespace ews {
    namespace test {
        /**
         * @ingroup Test
         * Test harness for precomputed potentials.
         */        
        class PrecomputedPotentialTest :  public QObject  {
        public:
            PrecomputedPotentialTest() { /* do nothing */ }
            virtual ~PrecomputedPotentialTest() { /* do nothing */ }
            Q_OBJECT
            private slots:
            // Code here will be called before any test case begins
            void initTestCase();
            // Code here will be called after all test cases finish
            void cleanupTestCase();
            // Code here will be called before each test case begins
            void init();
            // Code here will be called after each test case finishes
            void cleanup();
            /**
             * Tests that the PrecomputedPotential can handle a single constant potential
             */
            void CanHandleSingleConstantPotential();
            /**
             * Tests using a potential that's larger than the specified bounds.
             */
            void CanHandlePotentialOutsideItsBounds();
        };
    }
}

#endif // __PRECOMPUTEDPOTENTIALTEST_H
