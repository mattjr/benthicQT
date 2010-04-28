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

#include "CompositePotentialTest.h"
#include "ConstantPotential.h"
#include "WallPotential.h"

namespace ews {
    namespace test {
        const Real NO_POTENTIAL = 0;
        const Real HAS_POTENTIAL = 100;
        
        using osg::Vec2;
        void CompositePotentialTest::initTestCase() { /* do nothing */ }
        void CompositePotentialTest::cleanupTestCase() { /* do nothing */ }
        void CompositePotentialTest::init() { /* do nothing */ }
        void CompositePotentialTest::cleanup() { /* do nothing */ }
        
        void CompositePotentialTest::DefaultConstructorIsZero() {
            CompositePotential p;
            // Tests that the potential is zero (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), NO_POTENTIAL);
        }
        
        void CompositePotentialTest::CanHandleSingleConstantPotential() {
            CompositePotential p;
            const Real value = 3.14159f;
            ref_ptr<Potential> constP = new ConstantPotential(value);
            p.addPotential(constP.get());
            // Tests that the potential is what we assigned it (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), value);
        }

        void CompositePotentialTest::CanHandleTwoConstantPotentials() {
            CompositePotential p;
            const Real value = 3.14159f;
            ref_ptr<Potential> constP = new ConstantPotential(value);
            p.addPotential(constP.get());
            p.addPotential(constP.get());
            // Tests that the potential is what we assigned it (unsigned int values shouldn't matter)
            QCOMPARE(p.getPotential(2, 2), 2 * value);            
        }
        
        void CompositePotentialTest::MethodRemovePotentialWorks() {
            CompositePotential p;
            const Real value = 3.14159f;
            ref_ptr<Potential> constP = new ConstantPotential(value);
            p.addPotential(constP);
            p.addPotential(constP);
            QCOMPARE(p.getPotential(2, 2), 2 * value);
            p.removePotential(constP);
            QCOMPARE(p.getPotential(2, 2), value);
        }
        
        void CompositePotentialTest::CanHandleCompositePotentialPotential() {
            const Real size = 50.f;
            ref_ptr<Potential> east = new WallPotential(Vec2(size, 0.0), Vec2(size, size));
            ref_ptr<Potential> north = new WallPotential(Vec2(0.0, size), Vec2(size, size));
            ref_ptr<Potential> south = new WallPotential(Vec2(0.0, 0.0), Vec2(size, 0.0));
            ref_ptr<Potential> west = new WallPotential(Vec2(0.0, 0.0), Vec2(0.0, size));
            ref_ptr<CompositePotential> northSouth = new CompositePotential();
            northSouth->addPotential(north.get());
            northSouth->addPotential(south.get());
            ref_ptr<CompositePotential> eastWest = new CompositePotential();
            eastWest->addPotential(east.get());
            eastWest->addPotential(west.get());
            CompositePotential world;
            ref_ptr<Potential> p = northSouth;
            world.addPotential(p.get());
            p = eastWest;
            world.addPotential(p.get());
            QCOMPARE(world.getPotential(0.0, size / 2), HAS_POTENTIAL);
            QCOMPARE(world.getPotential(size / 2, 0.0), HAS_POTENTIAL);
            QCOMPARE(world.getPotential(size / 2, size), HAS_POTENTIAL);
            QCOMPARE(world.getPotential(size, size / 2), HAS_POTENTIAL);
            QCOMPARE(world.getPotential(size / 2, size / 2), NO_POTENTIAL);
        }
    }
}

using ews::test::CompositePotentialTest;
QTEST_APPLESS_MAIN(CompositePotentialTest)
