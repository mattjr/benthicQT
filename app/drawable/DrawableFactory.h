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
#ifndef __DRAWABLE_FACTORY_H
#define  __DRAWABLE_FACTORY_H

#include <QtCore>
#include <osg/Node>
#include "DrawableQtAdapter.h"

namespace ews {
    namespace app {
        namespace drawable {
            /**
             * Factory pattern for managing drawable objects associated with objects.
             */
            class DrawableFactory {
            public:
                /** Get the singleton instance of the DrawableFactory. */
                static DrawableFactory& instance();
                
                /** 
                 * Determine if there is a geometric association for
                 * the given data object. 
                 * @return geometric representation, or \c NULL if none.
                 */
                DrawableQtAdapter* createDrawableFor(QObject& model);
                
            private:
                DrawableFactory() {}
                DrawableFactory(const DrawableFactory&) {}
                void operator=(const DrawableFactory&) {}
            };
        }
    }
}

#endif // __DRAWABLE_FACTORY_H
