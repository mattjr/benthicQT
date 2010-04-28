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

#ifndef DripSource_H
#define DripSource_H

#include <QtCore>
#include <QtCore/QTimer>
#include <osg/Geometry>
#include <osg/Vec2>
#include "WaveModel.h"
#include "Oscillator.h"
#include "EWSDefine.h"
#include "MathUtils.h"

namespace ews {
    namespace app {
        namespace model {
            using ews::physics::WaveModel;
            using ews::physics::Oscillator;
            using ews::physics::OscillatorVal;
            using osg::Vec2;

            const QString DRIPSOURCE1("dripSource1");
            const QString DRIPSOURCE2("dripSource2");
            // Used for converting frequency dial value to period (in seconds)
            const OscillatorVal MIN_PERIOD = 0.33; // Seconds
            const OscillatorVal MAX_PERIOD = 1.25; // Seconds
            

            class SimulationState;
            
            /**
             * Contains the business logic for DripSource objects to be drawn on the screen, as well as
             * a reference to the Oscillator necessary for physics calculations.
             */
            class DripSource :  public QObject  {
                
                Q_OBJECT
                Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
                Q_PROPERTY(bool paused READ isPaused WRITE setPaused)
                Q_PROPERTY(Uint frequency READ getFrequency WRITE setFrequency)
                Q_PROPERTY(Uint amplitude READ getAmplitude WRITE setAmplitude)
                Q_PROPERTY(Vec2 position READ getPosition WRITE setPosition)
                
            public:
                /**
                 * Standard ctor.
                 */
                explicit DripSource(WaveModel& model, SimulationState * parent);
                
                /**
                 * Determine if this source is enabled or not.
                 */
                bool isEnabled() const {
                    return _enabled;
                }
                
                /**
                 * Get the drip frequency in millihertz. 
                 */
                Uint getFrequency() const {
                    return _frequency;
                }
                
                /**
                 * Compute the period in milliseconds
                 * @return Period in milliseconds
                 */
                Uint computeMillisecondPeriod() {
                    return 100000 / getFrequency();
                }
                
                /**
                 * Get the amplitude, (0, 100]
                 */
                Uint getAmplitude() const {
                    return static_cast<Uint>(_oscillator.getAmplitude() / ews::physics::MAX_AMPLITUDE * 100);
                }
                
                /**
                 * Get the position of the drip source in the X/Y plane.
                 * @return 2D location.
                 */
                osg::Vec2 getPosition() const {
                    return osg::Vec2(_oscillator.x(), _oscillator.y());
                }
                
                /**
                 * Get the physics simulation peer object.
                 */
                Oscillator& getOscillator() {
                    return _oscillator;
                }
                
                /**
                 * Is drip pulsing active.
                 */
                bool isPaused() const {
                    return _paused;
                }
                
                /** 
                 * Get the rendering color. 
                 */
                osg::Vec4 getColor() const {
                    return _color;
                }
                
                /**
                 * Reset drip source to default configuration.
                 */
                void reset();
                
                
                /**
                 * Gets the SimulationState associated with this drip source.
                 * @return Parent SimulationState.
                 */
                SimulationState* getSimulationState() const;
                
            public slots:
                
                /**
                 * Set the enabled state of this drip source.
                 */
                void setEnabled(bool state) {
                    if (_enabled != state) {
                        _enabled = state;
                        if (!REALISTIC_DRIP && _frequency > 0) {
                            _oscillator.setOscillateStatus(state);
                        }

                        emit enabledChanged(state);
                    }
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
                 
                /**
                 * Set the amplitude percentage
                 * @param amplitude value in (0, 100]
                 */
                void setAmplitude(Uint amplitude) {
                    _oscillator.setAmplitude(amplitude/100.0 * ews::physics::MAX_AMPLITUDE);
                    emit amplitudeChanged(amplitude);
                }
                
                /**
                 * Set the frequency in (0, 100]
                 */
                void setFrequency(Uint frequency) {
                    using ews::util::scale;
                    _frequency = frequency;
                    if (!REALISTIC_DRIP) {
                        if (frequency > 0) {
                            if (_enabled != _oscillator.getOscillateStatus()) {
                                _oscillator.setOscillateStatus(_enabled);
                            }
                            OscillatorVal freq = scale(1 / MAX_PERIOD, getFrequency(), 1 / MIN_PERIOD);
                            // The corresponding frequency here differs from when REALISTIC_DRIP is true                            
                            _oscillator.setPeriod(1 / freq);
                        }
                        else {
                            _oscillator.setOscillateStatus(false);
                            _oscillator.setPeriod(ews::physics::DEFAULT_PERIOD);
                        }
                    }
                    emit frequencyChanged(frequency);
                }
                
                /**
                 * Set the XY position of the drip source in water medium
                 * coordinates.
                 */
                void setPosition(const osg::Vec2& pos) {
                    _oscillator.setLocation(static_cast<Uint>(pos.x()),
                                            static_cast<Uint>(pos.y()));
                    emit positionChanged(pos);
                }
                
                /**
                 * Introduce a single drop into the system.
                 */
                void pulseDrip() {
                    emit drip(_oscillator.getAmplitude());
                }
                
                /** Set the rendering color. */
                void setColor(osg::Vec4 color) {
                    _color = color;
                    emit colorChanged(color);
                }
                
            private slots:
                /** Propagate data model values to timer. */
                void updateTimer();
                /** Fire a drip from oscillator. */
                void pokeOscillator();
                
            signals:
                void enabledChanged(bool);
                void pausedStateChanged(bool);
                void frequencyChanged(int);
                void amplitudeChanged(int);
                void positionChanged(osg::Vec2);
                void colorChanged(osg::Vec4);
                
                
                /**
                 * Fired with a value between (0,100] when
                 * a water drop is added to the system.
                 * @param amplitude strength value (0, 100].
                 */
                void drip(int amplitude);
                
            private:
                Q_DISABLE_COPY(DripSource)
                Oscillator _oscillator;
                /** This timer is only responsible for giving the user
                 *  feedback that the source is active. The Oscillator
                 *  takes care of its own triggering when it's updated with
                 *  the current time. */
                QTimer _timer;
                bool _paused;
                bool _enabled;
                Uint _frequency;
                osg::Vec4 _color;
            };
        }
    }
}



#endif // DripSource_H
