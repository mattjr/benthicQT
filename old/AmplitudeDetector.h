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


#ifndef __AMPLITUDE_DRAWABLE_H
#define __AMPLITUDE_DRAWABLE_H

#include "DrawableQtAdapter.h"
#include "PointSampler.h"
#include "Knob.h"
#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/Switch>

namespace ews {
    namespace  app {
        namespace drawable {
            using ews::app::model::PointSampler;
            
            class AmplitudeDetector : public DrawableQtAdapter {
                Q_OBJECT
            public:
                explicit AmplitudeDetector(PointSampler& sampler);
                
                PointSampler& getDataModel() const {
                    return _sampler;
                }
                
            public slots:
                void setEnabled(bool enabled);
                void setPosition(const osg::Vec2& pos);                
                
            protected:
                /** Protected to encourage use with ref_ptr. */
                virtual ~AmplitudeDetector();
                
            private:
                Q_DISABLE_COPY(AmplitudeDetector)
                void createGeom();
                PointSampler& _sampler;
                ref_ptr<Knob> _dragger;
                ref_ptr<PositionAttitudeTransform> _geom;
                ref_ptr<Switch> _switch;
                
                /** Private support class. */
                friend class SampleUpdater;
            };
        }
    }
}


#endif // __AMPLITUDE_DRAWABLE_H

