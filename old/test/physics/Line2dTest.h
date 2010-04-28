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

#include "Line2d.h"
using namespace ews::physics;
#include <osg/Vec2>
#include <QtTest/QtTest>
#include "EWSDefine.h"

namespace ews {
    namespace test {
        using osg::Vec2;
        
        /**
         * @ingroup Test
         * Test harness for testing Line2d.
         */        
        class Line2dTest :  public QObject  {
        public:
            Line2dTest():
            _x1(2.0f), _y1(3.0f), _p1(2.0f, 3.0f),
            _x2(-1.7f), _y2(91.1f), _p2(-1.7f, 91.1f), _l() {
                _l.setStart(_p1);
                _l.setEnd(_p2);
            }
            
            virtual ~Line2dTest() { /* do nothing */ }
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
             * Tests that the Line2d constructor works
             */
            void ConstructorWorks();
            /**
             * Tests that the Line2d get/setStart methods work
             */
            void MethodsGetSetStartWork();
            /**
             * Tests that the Line2d get/setEnd methods work
             */
            void MethodsGetSetEndWork();
            /**
             * Tests that the Line2d epsilon method works
             */
            void MethodEpsilonWorks();
            /**
             * Tests that the Line2d ptSegProjection method works
             */
            void MethodPtSegProjectionWorks();
        private:
            const Real _x1;
            const Real _y1;
            const Vec2 _p1;
            const Real _x2;
            const Real _y2;
            const Vec2 _p2;
            Line2d _l;
        };
    }
}

#endif // __CONSTANTPOTENTIALTEST_H
