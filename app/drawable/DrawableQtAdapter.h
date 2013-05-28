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
 * Modified by Matthew Johnson-Roberson 
 * from original source: Visualize Physics - Wave 
 * Copyright (C) 2012 Matthew Johnson-Roberson
 * See COPYING for license details
 */

#ifndef __DRAWABLE_ADAPTER_H
#define __DRAWABLE_ADAPTER_H

#include <QtCore>
#include <osg/PositionAttitudeTransform>
#include <osgManipulator/CommandManager>

namespace ews {
    namespace app {
        namespace drawable {
            /**
             *  Abstract base class for all geometry, serving as an adapter between the Qt and OSG worlds.
             */
            class DrawableQtAdapter :  public QObject, public osg::PositionAttitudeTransform  {
                Q_OBJECT
                
            public:
                /**
                 * Standard ctor.
                 */
                explicit DrawableQtAdapter();
                
                /** Get access to the singleton instance of the command manager. */
                static osgManipulator::CommandManager& manipCommandManager();
                
            protected:
                /** Protected to encourage use with ref_ptr. */
                virtual ~DrawableQtAdapter();
                
                
            private:
                Q_DISABLE_COPY(DrawableQtAdapter)
            };
            
            inline QDebug operator<<(QDebug dbg, const DrawableQtAdapter &ds) {
                dbg << ds.metaObject()->className() << "{";
                dbg << "name=" << ds.objectName() << ",";
                dbg << "}";
                return dbg;
            }
        }
    }
    
}

#endif // __DRAWABLE_ADAPTER_H


