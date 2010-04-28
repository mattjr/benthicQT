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

#include "Oscillator.h"
using namespace ews::physics;

#include <QtTest/QtTest>

namespace ews {
    namespace test {
        /**
         * @ingroup Test
         * Test harness for Oscillator.
         */        
        class OscillatorTest :  public QObject  {
        public:
            OscillatorTest(): _waveModel(50, 100) { /* do nothing */ }
            virtual ~OscillatorTest() { /* do nothing */ }
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
             * Tests that the Oscillator default constructor creates a valid object
             */
            void DefaultConstructor();
            /**
             * Tests that Oscillator's setRadius method works
             */
            void MethodSetRadiusWorks();
            /**
             * Tests that Oscillator's setPeriod method works
             */
            void MethodSetPeriodWorks();
            /**
             * Tests that Oscillator's setLocation method works
             */
            void MethodSetLocationWorks();
            /**
             * Tests that Oscillator's computeFrequency method works
             */
            void MethodComputeFrequencyWorks();
            /**
             * Tests that Oscillator's setAmplitude method works
             */
            void MethodSetAmplitudeWorks();
            /**
             * Tests that Oscillator's setOscillateStatus method works
             */
            void MethodSetOscillateStatusWorks();
            /**
             * Tests that Oscillator's firePulse method works (also tests updateTimeAndOscillator)
             */
            void MethodFirePulseWorks();
        private:
            WaveModel _waveModel;
        };
    }
}

#endif // __CONSTANTPOTENTIALTEST_H
