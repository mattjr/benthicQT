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

#include "WaveModelTest.h"
#include "EWSDebug.h"
#include "DampedClassicalWavePropagator.h"

namespace ews {
    namespace test {
        void WaveModelTest::initTestCase() { /* do nothing */ }
        void WaveModelTest::cleanupTestCase() { /* do nothing */ }
        void WaveModelTest::init() { /* do nothing */ }
        void WaveModelTest::cleanup() { /* do nothing */ }
        void WaveModelTest::ConstructorWorks() {
            WaveModel testWaveModel(50, 50);
            // Tests that the average value for the range x = [0..4], y = [0..4] is zero
            QCOMPARE(testWaveModel.computeAverageValue(2, 2, 2), 0.0f);
        }
        void WaveModelTest::MethodPropagateWorksInPlace() {
            WaveModel testWaveModel(50, 50);
            LatticeVal expected = 1.0;
            unsigned int x = 10;
            unsigned int y = 10;
            testWaveModel.setSourceValue(10, 10, expected);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            testWaveModel.propagate();
            LatticeVal prior = expected;
            LatticeVal priorPrior = prior;
            expected = MULT_FACTOR * (SELF_PROPAGATE * prior - MEMORY_TERM * priorPrior);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            LatticeVal neighbor = MULT_FACTOR * NEIGHBOR_PROPAGATE * prior;
            QCOMPARE(testWaveModel.getValue(x, y + 1), neighbor);
            LatticeVal diagNeigh = MULT_FACTOR * DIAG_PROPAGATE * prior;
            QCOMPARE(testWaveModel.getValue(x + 1, y + 1), diagNeigh);
            testWaveModel.propagate();
            priorPrior = prior;
            prior = expected;
            expected = MULT_FACTOR * (4 * (neighbor * NEIGHBOR_PROPAGATE + diagNeigh * DIAG_PROPAGATE) + 
                                      SELF_PROPAGATE * prior - MEMORY_TERM * priorPrior);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            LatticeVal priorNeigh = neighbor;
            neighbor = MULT_FACTOR * ((prior + 2 * diagNeigh) * NEIGHBOR_PROPAGATE + 
                                      priorNeigh * (2 * DIAG_PROPAGATE + SELF_PROPAGATE));
            QCOMPARE(testWaveModel.getValue(x, y + 1), neighbor);
            diagNeigh = MULT_FACTOR * (2 * priorNeigh * NEIGHBOR_PROPAGATE + 
                                       prior * DIAG_PROPAGATE + diagNeigh * SELF_PROPAGATE);
            QCOMPARE(testWaveModel.getValue(x + 1, y + 1), diagNeigh);
            testWaveModel.propagate();
            priorPrior = prior;
            prior = expected;
            expected = MULT_FACTOR * (4 * (neighbor * NEIGHBOR_PROPAGATE + diagNeigh * DIAG_PROPAGATE) + 
                                      SELF_PROPAGATE * prior - MEMORY_TERM * priorPrior);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
        }
        void WaveModelTest::MethodPropagateWorksAcrossSpace() {
            WaveModel testWaveModel(50, 50);
            LatticeVal expected = 1.0f;
            unsigned int x = 10;
            unsigned int y = 10;
            testWaveModel.setSourceValue(x, y, expected);
            QCOMPARE(testWaveModel.getValue(x, y), expected);
            x++;
            QBENCHMARK {
                for (; x < testWaveModel.getWidth(); x++) {
                    QCOMPARE(testWaveModel.getValue(x, y), 0.0f);
                    QCOMPARE(testWaveModel.getValue(x, y - 1), 0.0f);
                    QCOMPARE(testWaveModel.getValue(x, y + 1), 0.0f);
                    const LatticeVal priorDiag = testWaveModel.getValue(x - 1, y + 1);
                    testWaveModel.propagate();
                    expected *= NEIGHBOR_PROPAGATE;
                    expected += 2 * priorDiag * DIAG_PROPAGATE; // From both diagonals
                    expected *= MULT_FACTOR;
                    QCOMPARE(testWaveModel.getValue(x, y), expected);
                }
            }
        }
    }
}

using ews::test::WaveModelTest;
QTEST_APPLESS_MAIN(WaveModelTest)
