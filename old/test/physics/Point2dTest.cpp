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

#include "Point2dTest.h"
#include <osg/Vec2>
#include "EWSDefine.h"

namespace ews {
    namespace test {
        using osg::Vec2;
        
        void Point2dTest::initTestCase() { /* do nothing */ }
        void Point2dTest::cleanupTestCase() { /* do nothing */ }
        void Point2dTest::init() { /* do nothing */ }
        void Point2dTest::cleanup() { /* do nothing */ }
        
        void Point2dTest::CopyConstructorTakesVector() {
            Real x = 1235.352534f;
            Real y = -235.234165f;
            const Vec2 p(Vec2(x, y));
            QCOMPARE(p.x(), x);
            QCOMPARE(p.y(), y);
        }
    }
}

using ews::test::Point2dTest;
QTEST_APPLESS_MAIN(Point2dTest)
