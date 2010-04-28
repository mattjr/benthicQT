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

#include "Oscillator.h"
#include <cmath>
using std::cos;
using std::sin;
using std::max;
using std::min;
using std::sqrt;
#if !defined(M_PI)
#define M_PI 3.1415926535897932384626433832795
#endif // M_PI
#include "EWSDebug.h"

namespace ews {
    namespace physics {
        Oscillator::Oscillator(WaveModel& waveModel): _waveModel(waveModel), _x(DEFAULT_X),
        _y(waveModel.getLength() / 2), _radius(DEFAULT_RADIUS), _amplitude(DEFAULT_AMPLITUDE),
        _period(DEFAULT_PERIOD), _time(0.0), _phase(0.0), _oscillating(false), _inPulse(false) {
            resetPhase();
        }
        
        Oscillator::Oscillator(WaveModel& waveModel, unsigned int x, unsigned int y):
        _waveModel(waveModel), _x(x), _y(y), _radius(DEFAULT_RADIUS), _amplitude(DEFAULT_AMPLITUDE),
        _period(DEFAULT_PERIOD), _time(0.0), _phase(0.0), _oscillating(false), _inPulse(false) {
            resetPhase();
        }
        
        void Oscillator::updateTimeAndOscillator(OscillatorVal time) {
            _time = time;
            if (_oscillating) {
                if (_waveModel.getPotential(_x, _y) <= 0.0) {
                    const OscillatorVal value = getValue();
                    // Making these int here so that subtraction doesn't wrap around
                    const int x = static_cast<int>(_x);
                    const int y = static_cast<int>(_y);
                    const int r = static_cast<int>(_radius);
                    const int minX = max(0, x - r);
                    const int maxX = min(static_cast<int>(_waveModel.getWidth()) - 1, x + r);
                    const int minY = max(0, y - r);
                    const int maxY = min(static_cast<int>(_waveModel.getLength() - 1), y + r);
                    for (int i = minX; i <= maxX; i++) {
                        for (int j = minY; j <= maxY; j++) {
                            if (sqrt(static_cast<Real>((i - x) * (i - x)  + (j - y) * (j - y))) < _radius) {
                                _waveModel.setSourceValue(i, j, value);
                            }
                        }
                    }
                }
            }
            if (_inPulse && (getCosArg() + _phase >= 2 * M_PI)) {
//            if (_inPulse && (getCosArg() + _phase >=  M_PI)) {
                _inPulse = false;
                _oscillating = false;
                _phase = 0;
//                qDebug() << "Pulse over: _oscillating = " << _oscillating << ", _inPulse = " << _inPulse;
            }
        }
        
        void Oscillator::firePulse() {
            resetPhase();
            _oscillating = true;
            _inPulse = true;
//            qDebug() << "Firing pulse: _oscillating = " << _oscillating << ", _inPulse = " << _inPulse;
        }
        
        OscillatorVal Oscillator::getValue() const {
            return _amplitude * cos(getCosArg() + _phase);
        }
        
        void Oscillator::setOscillateStatus(bool shouldOscillate) {
            if (!_oscillating && shouldOscillate) {
                resetPhase();
            }
            _inPulse = false;
            _oscillating = shouldOscillate;
//            qDebug() << "_oscillating = " << _oscillating << ", _inPulse = " << _inPulse;
        }
        
        void Oscillator::resetPhase() {
            _phase = -getCosArg() + M_PI/2.0; // Puts getValue at its minimum
        }
        
        OscillatorVal Oscillator::getCosArg() const {
            return 2 * M_PI * _time / _period;
        }
    }
}
