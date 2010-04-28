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


#include "PointSampler.h"
#include "SimulationState.h"
#include "Lattice.h"
#include "EWSDebug.h"
#include "EWSDefine.h"
#include "SamplerSet.h"
#include <cassert>

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::DEFAULT_WINDOW_WIDTH;
            
            static const QColor _predefinedColors[] =  {
                QColor(Qt::red),
                QColor(Qt::green),
                QColor(Qt::cyan),
                QColor(Qt::magenta),
                QColor(Qt::gray),
                QColor(Qt::darkRed),
                QColor(Qt::darkGreen),
                QColor(Qt::darkCyan),
                QColor(Qt::darkMagenta),
                QColor(Qt::darkYellow),
                QColor(Qt::darkGray),
                QColor(Qt::darkBlue),
                QColor(Qt::blue),
            };
            
            static const int _numPredefinedColors = 13;
            static int _samplerCount = 0;
            
            PointSampler::PointSampler(const Lattice& lattice, SamplerSet* parent, Uint size)  
            : QObject(parent), _position(0, 0), _enabled(true),  _paused(false),
            _lattice(lattice), _history(size), _color(_predefinedColors[_samplerCount++%_numPredefinedColors]) {

            }
            
            PointSampler::~PointSampler() {
            }
            
            void PointSampler::sample() {
                using osg::Vec2;
                if(isEnabled() && !isPaused()) {
                    Vec2 pos = getPosition();
                    ews::physics::LatticeVal val = _lattice.computeAverageValue(static_cast<Uint>(pos.x()),
                                                                                static_cast<Uint>(pos.y()),
                                                                                DEFAULT_WINDOW_WIDTH);
                    _history.addValue(val);
                    emit sampleHistoryChanged(this);
                }
            }
            
            SimulationState* PointSampler::getSimulationState() const {
                QObject* obj = parent();
                SamplerSet* parent = qobject_cast<SamplerSet*>(obj);
                assert(parent);
                return parent->getSimulationState();
            }
        }
    }
}
