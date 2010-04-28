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

#include "LatticeTest.h"

namespace ews {
    namespace test {
        const unsigned int UNSIGNED_ZERO = 0;
        const LatticeVal LATTICE_ZERO = 0.0f;
        const LatticeVal LATTICE_ONE = 1.0f;
        const LatticeVal LATTICE_TWO = 2.0f;
        const LatticeVal LATTICE_THREE = 3.0f;
        const LatticeVal LATTICE_NINE = 9.0f;
        const LatticeVal LATTICE_15 = 15.0f;
        const LatticeVal LATTICE_18 = 18.0f;
        const LatticeVal LATTICE_25 = 25.0f;
        
        void LatticeTest::initTestCase() { /* do nothing */ }
        void LatticeTest::cleanupTestCase() { /* do nothing */ }
        void LatticeTest::init() { /* do nothing */ }
        void LatticeTest::cleanup() { /* do nothing */ }
        
        void LatticeTest::DefaultConstructorWorks() {
            Lattice nada;
            QCOMPARE(nada.getLength(), UNSIGNED_ZERO);
            QCOMPARE(nada.getWidth(), UNSIGNED_ZERO);
        }

        void LatticeTest::MethodComputeAverageValueWorksForNormalCases() {
            Lattice testLattice(5, 5);
            // Tests that the average value for the range x = [0..4], y = [0..4] is zero
            QCOMPARE(testLattice.computeAverageValue(2, 2, 2), LATTICE_ZERO);
            testLattice.setValue(2, 2, LATTICE_NINE);
            // "Averaging" range x = 2, y = 2
            QCOMPARE(testLattice.computeAverageValue(2, 2, 0), LATTICE_NINE);
            // Averaging range x = [1..3], y = [1..3]
            QCOMPARE(testLattice.computeAverageValue(2, 2, 1), LATTICE_ONE);
            // Averaging range x = [0..4], y = [0..4]
            QCOMPARE(testLattice.computeAverageValue(2, 2, 2), LATTICE_NINE / LATTICE_25);
        }
        
        void LatticeTest::MethodComputeAverageValueWorksForWindowsGoingOutsideBounds() {
            Lattice testLattice(5, 5);
            // Tests that the average value for the range x = [0..2], y = [0..2] is zero
            QCOMPARE(testLattice.computeAverageValue(0, 0, 2), LATTICE_ZERO);
            testLattice.setValue(2, 2, LATTICE_NINE);
            // Averaging range x = [0..1], y = [0..1]
            QCOMPARE(testLattice.computeAverageValue(0, 0, 1), LATTICE_ZERO);
            // Averaging range x = [0..2], y = [0..2]
            QCOMPARE(testLattice.computeAverageValue(0, 0, 2), LATTICE_ONE);
            // Averaging range x = [0..2], y = [0..4]
            QCOMPARE(testLattice.computeAverageValue(0, 2, 2), LATTICE_NINE / LATTICE_15);
            // Averaging range x = [0..4], y = [0..2]
            QCOMPARE(testLattice.computeAverageValue(2, 0, 2), LATTICE_NINE / LATTICE_15);
            // Averaging range x = [0..2], y = [2..4]
            QCOMPARE(testLattice.computeAverageValue(0, 4, 2), LATTICE_ONE);
            // Averaging range x = [2..4], y = [0..2]
            QCOMPARE(testLattice.computeAverageValue(4, 0, 2), LATTICE_ONE);
        }
        
        void LatticeTest::MethodGetWidthWorks() {
            QCOMPARE(Lattice(5, 3).getWidth(), static_cast<unsigned int>(5));
            QCOMPARE(Lattice(0, 1).getWidth(), UNSIGNED_ZERO);
            QCOMPARE(Lattice(1, 0).getWidth(), static_cast<unsigned int>(1));
        }
        
        void LatticeTest::MethodGetLengthWorks() {
            QCOMPARE(Lattice(5, 3).getLength(), static_cast<unsigned int>(3));
            QCOMPARE(Lattice(0, 1).getLength(), static_cast<unsigned int>(1));
            QCOMPARE(Lattice(1, 0).getLength(), UNSIGNED_ZERO);
        }
        
        void LatticeTest::MethodSetSizeWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, LATTICE_NINE);
            testLattice.setSize(10, 20);
            QCOMPARE(testLattice.getWidth(), static_cast<unsigned int>(10));
            QCOMPARE(testLattice.getLength(), static_cast<unsigned int>(20));
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_ZERO);
            testLattice.setSize(0, 0);
        }
        
        void LatticeTest::MethodSetGetValueWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, LATTICE_NINE);
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_NINE);
        }

        void LatticeTest::MethodScaleWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, LATTICE_NINE);
            testLattice.scale(LATTICE_TWO);
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_18);
        }
        
        void LatticeTest::MethodScaleLocationWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, LATTICE_NINE);
            testLattice.scaleLocation(2, 1, LATTICE_TWO);
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_NINE);
            testLattice.scaleLocation(2, 2, LATTICE_TWO);
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_18);
        }
        
        void LatticeTest::MethodClearWorks() {
            Lattice testLattice(5, 5);
            testLattice.setValue(2, 2, LATTICE_NINE);
            testLattice.clear();
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_ZERO);
        }
        
        void LatticeTest::AssignmentOperatorWorks() {
            Lattice testLattice(15, 5);
            testLattice.setValue(2, 1, LATTICE_THREE);
	    // This uses a copy constructor, not the assignment operator. To test that the copy constructor
	    // is disabled. Uncomment the following line. It should not compile.
            //Lattice copy = testLattice;
            Lattice copy(1, 1);
            copy = testLattice;
            testLattice.setValue(2, 2, LATTICE_NINE);
            QCOMPARE(testLattice.getValue(2, 2), LATTICE_NINE);            
            QCOMPARE(copy.getValue(2, 2), LATTICE_ZERO);            
            QCOMPARE(copy.getValue(2, 1), LATTICE_THREE);            
            QCOMPARE(copy.getWidth(), testLattice.getWidth());
            QCOMPARE(copy.getLength(), testLattice.getLength());
        }
    }
}

using ews::test::LatticeTest;
QTEST_APPLESS_MAIN(LatticeTest)
