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

#include "ConstantPotentialTest.h"

namespace ews {
    namespace test {
        void ConstantPotentialTest::initTestCase() { /* do nothing */ }
        void ConstantPotentialTest::cleanupTestCase() { /* do nothing */ }
        void ConstantPotentialTest::init() { /* do nothing */ }
        void ConstantPotentialTest::cleanup() { /* do nothing */ }
        
        void ConstantPotentialTest::DefaultConstructorIsZero() {
            ConstantPotential p;
            // Tests that the potential is zero (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), static_cast<Real>(0.0));
        }
        
        void ConstantPotentialTest::ValueConstructorYieldsValue() {
            Real value = 3.14159f;
            ConstantPotential p(value);
            // Tests that the potential is what we assigned it (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), value);
        }
    }
}

using ews::test::ConstantPotentialTest;
QTEST_APPLESS_MAIN(ConstantPotentialTest)
