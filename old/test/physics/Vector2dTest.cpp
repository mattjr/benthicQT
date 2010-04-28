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

#include "Vector2dTest.h"
#include <osg/Vec2>
#include "EWSDefine.h"

namespace ews {
    namespace test {
        using osg::Vec2;
        
        void Vector2dTest::initTestCase() { /* do nothing */ }
        void Vector2dTest::cleanupTestCase() { /* do nothing */ }
        void Vector2dTest::init() { /* do nothing */ }
        void Vector2dTest::cleanup() { /* do nothing */ }
        
        void Vector2dTest::CopyConstructorTakesPoints() {
            const Real x = -1234.51235f;
            const Real y = 3235235253.2343f;
            const Vec2 v(Vec2(x, y));
            QCOMPARE(v.x(), x);
            QCOMPARE(v.y(), y);
        }
        
        void Vector2dTest::MethodLengthWorks() {
            const Vec2 v(5.0f, 12.0f);
            QCOMPARE(v.length(), 13.0f);
        }
        
        void Vector2dTest::MethodLengthSqWorks() {
            const Vec2 v(1.0f, 2.1f);
            QCOMPARE(v.length2(), 5.41f);
        }
        
        void Vector2dTest::MethodDotWorks() {
            const Vec2 v1(0.0f, 3.0f);
            const Vec2 v2(4.0f, 0.0f);
            QCOMPARE(v1 * v2, 0.0f);
            const Vec2 v3(0.0f, 4.0f);
            QCOMPARE(v1 * v3, 12.0f);
            const Vec2 v4(0.0f, -4.0f);
            QCOMPARE(v1 * v4, -12.0f);
            const Vec2 v5(3.14159f, -4.0f);
            QCOMPARE(v1 * v5, -12.0f);
        }
    }
}

using ews::test::Vector2dTest;
QTEST_APPLESS_MAIN(Vector2dTest)
