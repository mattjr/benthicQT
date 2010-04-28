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

#ifndef __POINT_SAMPLER_H
#define __POINT_SAMPLER_H

#include <QtCore>
#include <QColor>
#include <osg/Vec2>
#include "Lattice.h"
#include "EWSDefine.h"
#include <osg/BoundsChecking>
#include "SampleHistory.h"

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::Lattice;
            using ews::physics::LatticeVal;
            using ews::Uint;
            
            class SimulationState;
            class SamplerSet;
            
            /** This class is responsible for sampling the lattice when its
             * \c sample() method is called, and maintaining a history of
             * values up to a specified history length.
             */
            class PointSampler : public QObject {
                Q_OBJECT
                
            public:
                /**
                 * Standard ctor.
                 * @param lattice The lattice from which samples should be taken.
                 * @param size the total size of the history buffer.
                 * @param parent Qt parent. 
                 */
                PointSampler(const Lattice& lattice, SamplerSet* parent, Uint size = 512); 
                
                virtual ~PointSampler();
                
                /**
                 * Get location in lattice. 
                 */
                osg::Vec2 getPosition() const {
                    return _position;
                }
                
                /**
                 * Determine if sampler is active.
                 */
                bool isEnabled() const {
                    return _enabled;
                }
                
                /**
                 * Is sampling paused?
                 */
                bool isPaused() const {
                    return _paused;
                }
                
                /**
                 * Get the rendering color
                 */
                const QColor& getColor() const {
                    return _color;
                }
                
                /**
                 * Get read-only access to history data.
                 */
                const SampleHistory& getHistory() const {
                    return _history;
                }
                
                /**
                 * Gets the SimulationState parent associated with this.
                 * @return Parent SimulationState.
                 */
                SimulationState* getSimulationState() const;
                
            public slots:
                /** Set the location in the lattice where sampling should occur. */
                void setPosition(osg::Vec2 pos) {
                    // Clamp to have values inside lattice.
                    osg::clampArrayElementBetweenRange<osg::Vec2, Uint>(pos, 0, 0, (Uint)_lattice.getWidth() - 1, "sampler x position");
                    osg::clampArrayElementBetweenRange<osg::Vec2, Uint>(pos, 1, 0, (Uint)_lattice.getLength() - 1, "sampler y position");
                    _position = pos;
                    emit positionChanged(_position);
                }

                /** Change enabled state of sampling. */
                void setEnabled(bool state) {
                    _enabled = state;
                    emit enabledChanged(state);
                }
                
                /**
                 * Change the rendering color. 
                 */
                void setColor(const QColor& c) {
                    _color = c;
                    emit colorChanged(_color);
                }
                
                /**
                 * Set the paused state.
                 */
                void setPaused(bool state) {
                    if (_paused != state) {
                        _paused = state;
                        emit pausedStateChanged(state);
                    }
                }
                
                /** Perform a lattice sample operation and store in history. */
                void sample();
                
                /** Clear history data. */
                void reset() {
                    _history.clear();
                    emit sampleHistoryChanged(this);
                }
                
            signals:
                /** Fired when the location changes. */
                void positionChanged(osg::Vec2);
                /** Fired when enabled state changes. */
                void enabledChanged(bool);
                /** Fired when the history of samples has changed. */
                void sampleHistoryChanged(const PointSampler*);
                /** Fired when the paused state changes. */
                void pausedStateChanged(bool);
                /** Fired when color changes. */
                void colorChanged(QColor);
                
            private:
                Q_DISABLE_COPY(PointSampler);
                
                /** Lattice position. */
                osg::Vec2 _position;
                /** Whether sampler does anything. */
                bool _enabled;
                /** Whether the sampling of data is halted. */
                bool _paused;
                /** Reference to object to sample. */
                const Lattice& _lattice;
                /** History of previous samples. */
                SampleHistory _history;
                /** Rendering color. */
                QColor _color;
            };
        }
    }
}

#endif // __POINT_SAMPLER_H
