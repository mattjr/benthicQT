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

#ifndef __LATTICETEST_H
#define __LATTICETEST_H

#include "Lattice.h"
using namespace ews::physics;

#include <QtTest/QtTest>

namespace ews {
    namespace test {
        /**
         * @ingroup Test
         * Test harness for testing the Lattice class.
         */        
        class LatticeTest :  public QObject  {
        public:
            LatticeTest() { /* do nothing */ }
            virtual ~LatticeTest() { /* do nothing */ }
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
             * Verifies that we can create an empty lattice of zero size.
             */
            void DefaultConstructorWorks();
            /**
             * Tests that the Lattice::computeAverageValue() method works for normal cases.
             */
            void MethodComputeAverageValueWorksForNormalCases();
            /**
             * Tests that the Lattice::computeAverageValue() method works for cases where windows go outside bounds.
             */
            void MethodComputeAverageValueWorksForWindowsGoingOutsideBounds();
            /**
             * Tests that the Lattice::getWidth() method works
             */
            void MethodGetWidthWorks();
            /**
             * Tests that the Lattice::getLength() method works
             */
            void MethodGetLengthWorks();
            /**
             * Tests that the Lattice::setSize() method works
             */
            void MethodSetSizeWorks();
            /**
             * Tests that Lattice::setValue() and Lattice::getValue() method works
             */
            void MethodSetGetValueWorks();
            /**
             * Tests that the Lattice::scale() method works
             */
            void MethodScaleWorks();
            /**
             * Tests that the Lattice::scaleLocation() method works
             */
            void MethodScaleLocationWorks();
            /**
             * Tests that the Lattice::clear() method works
             */
            void MethodClearWorks();
            /**
             * Tests that the assignment operator does a deep copy.
             */
            void AssignmentOperatorWorks();
        };
    }
}

#endif // __LATTICETEST_H
