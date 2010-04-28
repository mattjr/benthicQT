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

#include "OscillatorTest.h"
#include <cmath>
using std::abs;
using std::cos;

namespace ews {
    namespace test {
        const OscillatorVal NADA = 0;
        
        void OscillatorTest::initTestCase() { /* do nothing */ }
        void OscillatorTest::cleanupTestCase() { /* do nothing */ }
        void OscillatorTest::init() { /* do nothing */ }
        void OscillatorTest::cleanup() { /* do nothing */ }
        
        void OscillatorTest::DefaultConstructor() {
            Oscillator testOscillator(_waveModel);
//            QCOMPARE(testOscillator.getValue(), 0.0);
            QVERIFY(abs(testOscillator.getValue()) < 1e-16);
            // I don't like that we're not precise enough for qtest's QCOMPARE
            if (abs(testOscillator.getValue()) > 1e-18) {
                QWARN("Value was greater than expected");
            }
            QCOMPARE(testOscillator.getRadius(), DEFAULT_RADIUS);
            QCOMPARE(testOscillator.getPeriod(), DEFAULT_PERIOD);            
            QCOMPARE(testOscillator.x(), DEFAULT_X);
            QCOMPARE(testOscillator.y(), _waveModel.getLength() / 2);            
            QCOMPARE(testOscillator.getAmplitude(), DEFAULT_AMPLITUDE);
            QCOMPARE(testOscillator.getTime(), NADA);            
            QCOMPARE(testOscillator.getOscillateStatus(), false);         
        }
        void OscillatorTest::MethodSetRadiusWorks() {
            Oscillator testOscillator(_waveModel);
            OscillatorVal expected = 2.5;
            testOscillator.setRadius(expected);
            QCOMPARE(testOscillator.getRadius(), expected);
            testOscillator.setRadius(0.0);
            QCOMPARE(testOscillator.getRadius(), NADA);
            testOscillator.setRadius(-0.5);
            QCOMPARE(testOscillator.getRadius(), NADA);
        }
        void OscillatorTest::MethodSetPeriodWorks() {
            Oscillator testOscillator(_waveModel);
            OscillatorVal expected = 2.5;
            testOscillator.setPeriod(expected);
            QCOMPARE(testOscillator.getPeriod(), expected);            
            testOscillator.setPeriod(0.0);
            QCOMPARE(testOscillator.getPeriod(), NADA);            
            testOscillator.setPeriod(-0.5);
            QCOMPARE(testOscillator.getPeriod(), NADA);            
        }
        void OscillatorTest::MethodSetLocationWorks() {
            Oscillator testOscillator(_waveModel);
            unsigned int x = 5;
            unsigned int y = 20;
            testOscillator.setLocation(x, y);
            QCOMPARE(testOscillator.x(), x);            
            QCOMPARE(testOscillator.y(), y);            
        }
        void OscillatorTest::MethodComputeFrequencyWorks() {
            Oscillator testOscillator(_waveModel);
            OscillatorVal expected = 2;
            testOscillator.setPeriod(expected);
            QCOMPARE(testOscillator.computeFrequency(), 1 / expected);            
        }
        void OscillatorTest::MethodSetAmplitudeWorks() {
            Oscillator testOscillator(_waveModel);
            OscillatorVal expected = 2.5;
            testOscillator.setAmplitude(expected);
            QCOMPARE(testOscillator.getAmplitude(), expected);            
            testOscillator.setAmplitude(0.0);
            QCOMPARE(testOscillator.getAmplitude(), NADA);            
            testOscillator.setAmplitude(-0.5);
            QCOMPARE(testOscillator.getAmplitude(), NADA);
        }
        void OscillatorTest::MethodSetOscillateStatusWorks() {
            Oscillator testOscillator(_waveModel);
            QCOMPARE(testOscillator.getOscillateStatus(), false);         
            testOscillator.setOscillateStatus(true);
            QCOMPARE(testOscillator.getOscillateStatus(), true);         
            testOscillator.setOscillateStatus(false);
            QCOMPARE(testOscillator.getOscillateStatus(), false);         
        }
        void OscillatorTest::MethodFirePulseWorks() {
            Oscillator testOscillator(_waveModel);
            unsigned int x = 5;
            unsigned int y = 5;
            testOscillator.setLocation(x, y);
            testOscillator.setRadius(2);
            testOscillator.setPeriod(1);
            testOscillator.setAmplitude(1);
            OscillatorVal time = 100.0;
            testOscillator.updateTimeAndOscillator(time);
            testOscillator.firePulse();
            const OscillatorVal deltaT = 0.1;
            time += deltaT;
            testOscillator.updateTimeAndOscillator(time);
            LatticeVal expected = cos(2 * M_PI * deltaT + (M_PI / 2.0));
            QCOMPARE(_waveModel.getValue(x, y), expected);
            QCOMPARE(_waveModel.getValue(x + 1, y + 1), expected);
            QCOMPARE(_waveModel.getValue(x + 2, y + 2), static_cast<LatticeVal>(0));
        }
    }
}

using ews::test::OscillatorTest;
QTEST_APPLESS_MAIN(OscillatorTest)
