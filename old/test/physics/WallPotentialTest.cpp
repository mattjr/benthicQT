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

#include "WallPotentialTest.h"
#include <osg/Vec2>

namespace ews {
    namespace test {
        const Real NO_POTENTIAL = 0;
        const Real HAS_POTENTIAL = 100;
        
        using osg::Vec2;
        void WallPotentialTest::initTestCase() { /* do nothing */ }
        void WallPotentialTest::cleanupTestCase() { /* do nothing */ }
        void WallPotentialTest::init() { /* do nothing */ }
        void WallPotentialTest::cleanup() { /* do nothing */ }
        
        void WallPotentialTest::DefaultConstructorIsPointWall() {
            WallPotential p;
            // Tests that the potential is zero away from origin
            QCOMPARE(p.getPotential(2, 2), NO_POTENTIAL);
            // Tests that the potential is 100 at origin
            QCOMPARE(p.getPotential(0, 0), HAS_POTENTIAL);
        }
        
        void WallPotentialTest::PreferredConstructorWorks() {
            WallPotential p(Vec2(0.0, 0.0), Vec2(5.0, 5.0));
            QCOMPARE(p.getPotential(2, 2), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(6, 6), NO_POTENTIAL);
            QCOMPARE(p.getPotential(2, 3), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(3, 2), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(4, 2), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(5, 0), NO_POTENTIAL);
        }
        
        void WallPotentialTest::MethodsGetSetSrcPointWork() {
            WallPotential p(Vec2(0.0, 0.0), Vec2(5.0, 5.0));
            p.setSrcPoint(Vec2(10.0, -10.0));
            QCOMPARE(p.getSrcPoint().x(), 10.0f);
            QCOMPARE(p.getSrcPoint().y(), -10.0f);
        }
        
        void WallPotentialTest::MethodsGetSetDstPointWork() {
            WallPotential p(Vec2(0.0, 0.0), Vec2(5.0, 5.0));
            p.setDstPoint(Vec2(10.0, -10.0));
            QCOMPARE(p.getDstPoint().x(), 10.0f);
            QCOMPARE(p.getDstPoint().y(), -10.0f);
        }
        
        void WallPotentialTest::MethodGetSetThicknessWorks() {
            WallPotential p(Vec2(0.0, 0.0), Vec2(5.0, 5.0));
            const Real expected = 2;
            p.setThickness(expected);
            QCOMPARE(p.getThickness(), expected);
            QCOMPARE(p.getPotential(2, 2), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(3, 2), HAS_POTENTIAL);
            QCOMPARE(p.getPotential(4, 2), NO_POTENTIAL);
        }
    }
}

using ews::test::WallPotentialTest;
QTEST_APPLESS_MAIN(WallPotentialTest)
