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

#ifndef __CONSTANTPOTENTIALTEST_H
#define __CONSTANTPOTENTIALTEST_H

#include "Tuple2d.h"
using namespace ews::physics;

#include <QtTest/QtTest>

namespace ews {
    namespace test {
        /**
         * @ingroup Test
         * Test harness for Tuple2d.
         */        
        class Tuple2dTest :  public QObject  {
        public:
            Tuple2dTest() { /* do nothing */ }
            virtual ~Tuple2dTest() { /* do nothing */ }
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
             * Tests that the Tuple2d::Tuple2d() constructor works
             */
            void DefaultConstructorIsZero();
            /**
             * Tests that the Tuple2d::Tuple2d(Real x, Real y) constructor works
             */
            void XYConstructorYieldsXY();
            /**
             * Tests that the Tuple2d::Tuple2d(Tuple2d t1, Tuple2d t2, Real alpha) constructor works
             */
            void InterpolatedConstructorIsInterpolated();
            /**
             * Tests that the Tuple2d::Tuple2d(Tuple2d t1) constructor works
             */
            void CopyConstructorCopies();
            /**
             * Tests that the Tuple2d::computeEpsilon(Tuple2d t1, Tuple2d t2) works
             */
            void MethodComputeEpsilonWorks();
            /**
             * Tests that the Tuple2d assignment operator works
             */
            void OperatorAssignmentWorks();
            /**
             * Tests that the Tuple2d subtraction operator works
             */
            void OperatorSubtractionWorks();
        };
    }
}

#endif // __CONSTANTPOTENTIALTEST_H
