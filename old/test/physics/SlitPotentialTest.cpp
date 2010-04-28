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

#include "SlitPotentialTest.h"
#include <osg/Vec2>

using namespace ews::physics;

namespace ews {
    namespace test {
        using osg::Vec2;
        const Real NO_POTENTIAL = 0;
        const Real HAS_POTENTIAL = 100;
        
        void SlitPotentialTest::initTestCase() { /* do nothing */ }
        void SlitPotentialTest::cleanupTestCase() { /* do nothing */ }
        void SlitPotentialTest::init() { /* do nothing */ }
        void SlitPotentialTest::cleanup() { /* do nothing */ }
        
        void SlitPotentialTest::PreferredConstructorWorks() {
            SlitPotential p(Vec2(0.0, 0.0), Vec2(6.0, 6.0), 2);
            QCOMPARE(p.getSlitLocation(0), Vec2(2.0, 2.0));
            QCOMPARE(p.getSlitLocation(1), Vec2(4.0, 4.0));
        }
        
        void SlitPotentialTest::MethodAddSlitWorks() {
            SlitPotential p(Vec2(0.0, 0.0), Vec2(5.0, 5.0));
            p.addSlit(0.5);
            QCOMPARE(p.getSlitLocation(0), Vec2(2.5, 2.5));
        }
        
        void SlitPotentialTest::MethodGetSlitLocationWorks() {
            SlitPotential p(Vec2(0.0, 0.0), Vec2(5.0, 5.0));
            p.addSlit(0.5);
            p.addSlit(0.25);
            p.addSlit(0.75);
            QCOMPARE(p.getSlitLocation(0), Vec2(1.25, 1.25));
            QCOMPARE(p.getSlitLocation(1), Vec2(2.5, 2.5));
            QCOMPARE(p.getSlitLocation(2), Vec2(3.75, 3.75));
        }
        
        void SlitPotentialTest::MethodGetPotentialWorks() {
            SlitPotential p(Vec2(60.0, 0.0), Vec2(60.0, 60.0), 2);
            QCOMPARE(p.getPotential(60.0, 10.0), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(60.0, 20.0), NO_POTENTIAL);
            QCOMPARE(p.getPotential(50.0, 10.0), NO_POTENTIAL);
        }
        
        void SlitPotentialTest::MethodRemoveSlitWorks() {
            SlitPotential p(Vec2(0.0, 0.0), Vec2(6.0, 6.0), 2);
            QCOMPARE(p.getSlitLocation(0), Vec2(2.0, 2.0));
            QCOMPARE(p.getSlitLocation(1), Vec2(4.0, 4.0));
            p.removeSlit(0);
            QCOMPARE(p.getSlitLocation(0), Vec2(4.0, 4.0));
            p.removeSlit(1); // Should do nothing since we now only have one slit
            QCOMPARE(p.getSlitLocation(0), Vec2(4.0, 4.0));
        }
        void SlitPotentialTest::MethodGetSlitWidthWorks() {
            SlitPotential p(Vec2(0.0, 0.0), Vec2(6.0, 6.0), 2);
            QCOMPARE(p.getSlitWidth(), DEFAULT_SLIT_WIDTH);
            const Real newSlitWidth = 1.1 * DEFAULT_SLIT_WIDTH;
            p.setSlitWidth(newSlitWidth);
            QCOMPARE(p.getSlitWidth(), newSlitWidth);
        }
        void SlitPotentialTest::MethodSetSlitWidthWorks() {
            SlitPotential p(Vec2(0.0, 0.0), Vec2(6.0, 6.0), 2);
            QCOMPARE(p.getSlitWidth(), DEFAULT_SLIT_WIDTH);
            const Real newSlitWidth = 1.1 * DEFAULT_SLIT_WIDTH;
            p.setSlitWidth(newSlitWidth);
            QCOMPARE(p.getSlitWidth(), newSlitWidth);
            p.setSlitWidth(-newSlitWidth);
            QCOMPARE(p.getSlitWidth(), 0.f);
        }
    }
}

using ews::test::SlitPotentialTest;
QTEST_APPLESS_MAIN(SlitPotentialTest)
