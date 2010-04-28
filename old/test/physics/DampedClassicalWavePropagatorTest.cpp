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

#include "DampedClassicalWavePropagatorTest.h"
#include "ConstantPotential.h"

namespace ews {
    namespace test {
        
        void DampedClassicalWavePropagatorTest::initTestCase() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::cleanupTestCase() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::init() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::cleanup() { /* do nothing */ }
        void DampedClassicalWavePropagatorTest::ConstructorWorks() {
            ref_ptr<Potential> p = new ConstantPotential();
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p, 50, 50, 20, 20);
        }
        void DampedClassicalWavePropagatorTest::MethodPropagateWorksInPlace() {
            unsigned int width = 50;
            unsigned int length = 50;
            Lattice lattice(width, length);
            ref_ptr<Potential> p = new ConstantPotential();
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p.get(), width, length, 20, 20);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            testDampedClassicalWavePropagator.propagate(lattice);
            LatticeVal prior = expected;
            LatticeVal priorPrior = prior;
            expected = MULT_FACTOR * (SELF_PROPAGATE * prior - MEMORY_TERM * priorPrior);
            QCOMPARE(lattice.getValue(x, y), expected);
            LatticeVal neighbor = MULT_FACTOR * NEIGHBOR_PROPAGATE * prior;
            QCOMPARE(lattice.getValue(x, y + 1), neighbor);
            LatticeVal diagNeigh = MULT_FACTOR * DIAG_PROPAGATE * prior;
            QCOMPARE(lattice.getValue(x + 1, y + 1), diagNeigh);
            testDampedClassicalWavePropagator.propagate(lattice);
            priorPrior = prior;
            prior = expected;
            expected = MULT_FACTOR * (4 * (neighbor * NEIGHBOR_PROPAGATE + diagNeigh * DIAG_PROPAGATE) + 
                                      SELF_PROPAGATE * prior - MEMORY_TERM * priorPrior);
            QCOMPARE(lattice.getValue(x, y), expected);
            LatticeVal priorNeigh = neighbor;
            neighbor = MULT_FACTOR * ((prior + 2 * diagNeigh) * NEIGHBOR_PROPAGATE + 
                                      priorNeigh * (2 * DIAG_PROPAGATE + SELF_PROPAGATE));
            QCOMPARE(lattice.getValue(x, y + 1), neighbor);
            diagNeigh = MULT_FACTOR * (2 * priorNeigh * NEIGHBOR_PROPAGATE + prior * DIAG_PROPAGATE + 
                                       diagNeigh * SELF_PROPAGATE);
            QCOMPARE(lattice.getValue(x + 1, y + 1), diagNeigh);
            testDampedClassicalWavePropagator.propagate(lattice);
            priorPrior = prior;
            prior = expected;
            expected = MULT_FACTOR * (4 * (neighbor * NEIGHBOR_PROPAGATE + diagNeigh * DIAG_PROPAGATE) + 
                                      SELF_PROPAGATE * prior - MEMORY_TERM * priorPrior);
            QCOMPARE(lattice.getValue(x, y), expected);
        }
        void DampedClassicalWavePropagatorTest::MethodPropagateWorksAcrossSpace() {
            unsigned int width = 50;
            unsigned int length = 50;
            Lattice lattice(width, length);
            ref_ptr<Potential> p = new ConstantPotential();
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p.get(), width, length, 20, 20);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            x++;
            const LatticeVal zero = 0.0f;
            for (; x < lattice.getWidth(); x++) {
                QCOMPARE(lattice.getValue(x, y), zero);
                QCOMPARE(lattice.getValue(x, y - 1), zero);
                QCOMPARE(lattice.getValue(x, y + 1), zero);
                const LatticeVal priorDiag = lattice.getValue(x - 1, y + 1);
                testDampedClassicalWavePropagator.propagate(lattice);
                expected *= NEIGHBOR_PROPAGATE;
                expected += 2 * priorDiag * DIAG_PROPAGATE; // From both diagonals
                expected *= MULT_FACTOR;
                QCOMPARE(lattice.getValue(x, y), expected);
            }
        }
        void DampedClassicalWavePropagatorTest::BenchmarkPropagate() {
            unsigned int width = 1000;
            unsigned int length = 1000;
            Lattice lattice(width, length);
            ref_ptr<Potential> p = new ConstantPotential();
            DampedClassicalWavePropagator testDampedClassicalWavePropagator(p.get(), width, length, 20, 20);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            lattice.setValue(x, y, expected);
            testDampedClassicalWavePropagator.setBoundaryCondition(x, y, expected);
            QBENCHMARK {
                for (; x < lattice.getWidth(); x++) {
                    testDampedClassicalWavePropagator.propagate(lattice);
                }
            }
        }
    }
}

using ews::test::DampedClassicalWavePropagatorTest;
QTEST_APPLESS_MAIN(DampedClassicalWavePropagatorTest)
