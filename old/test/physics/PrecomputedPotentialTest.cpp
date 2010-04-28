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

#include "PrecomputedPotentialTest.h"
#include "ConstantPotential.h"
#include "WallPotential.h"
#include "CompositePotential.h"
#include <osg/Vec2>

namespace ews {
    namespace test {
        using osg::Vec2;
        const Real NO_POTENTIAL = 0;
        const Real HAS_POTENTIAL = 100;
        
        void PrecomputedPotentialTest::initTestCase() { /* do nothing */ }
        void PrecomputedPotentialTest::cleanupTestCase() { /* do nothing */ }
        void PrecomputedPotentialTest::init() { /* do nothing */ }
        void PrecomputedPotentialTest::cleanup() { /* do nothing */ }
        
        void PrecomputedPotentialTest::CanHandleSingleConstantPotential() {
            const Real value = 3.14159f;
            ref_ptr<Potential> constP = new ConstantPotential(value);
            PrecomputedPotential p(constP, 10, 10);
            QCOMPARE(p.getPotential(2, 2), value);
            QCOMPARE(p.getPotential(11, 2), NO_POTENTIAL);
            QCOMPARE(p.getPotential(2, 11), NO_POTENTIAL);
        }
        
        void PrecomputedPotentialTest::CanHandlePotentialOutsideItsBounds() {
            const Real size = 50.f;
            const Real width = 6.f;
            ref_ptr<Potential> east = new WallPotential(Vec2(size, 0.0), Vec2(size, size), width);
            ref_ptr<Potential> north = new WallPotential(Vec2(0.0, size), Vec2(size, size), width);
            ref_ptr<Potential> south = new WallPotential(Vec2(0.0, 0.0), Vec2(size, 0.0), width);
            ref_ptr<Potential> west = new WallPotential(Vec2(0.0, 0.0), Vec2(0.0, size), width);
            ref_ptr<CompositePotential> northSouth = new CompositePotential();
            northSouth->addPotential(north.get());
            northSouth->addPotential(south.get());
            ref_ptr<CompositePotential> eastWest = new CompositePotential();
            eastWest->addPotential(east.get());
            eastWest->addPotential(west.get());
            ref_ptr<CompositePotential> world = new CompositePotential();
            ref_ptr<Potential> p = northSouth;
            world->addPotential(p.get());
            p = eastWest;
            world->addPotential(p.get());
            p = world;
            // Notice that bounds are slightly smaller than potential being passed to it
            PrecomputedPotential pre(p.get(), static_cast<unsigned int>(size - 1), static_cast<unsigned int>(size - 1));
            QCOMPARE(pre.getPotential(0.0, size / 2), HAS_POTENTIAL);
            QCOMPARE(pre.getPotential(size / 2, 0.0), HAS_POTENTIAL);
            QCOMPARE(pre.getPotential(size / 2, size), NO_POTENTIAL); // Because this is outside the precomputed bounds
            QCOMPARE(pre.getPotential(size, size / 2), NO_POTENTIAL); // Because this is outside the precomputed bounds
            QCOMPARE(pre.getPotential(size / 2, size - 2), HAS_POTENTIAL); // However, the wall can be felt from here
            QCOMPARE(pre.getPotential(size - 2, size / 2), HAS_POTENTIAL);
            QCOMPARE(pre.getPotential(size / 2, size / 2), NO_POTENTIAL);
        }
    }
}

using ews::test::PrecomputedPotentialTest;
QTEST_APPLESS_MAIN(PrecomputedPotentialTest)
