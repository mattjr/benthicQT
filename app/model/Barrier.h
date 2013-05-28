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

#ifndef __BARRIER_H
#define __BARRIER_H


#include <QtCore>
#include <QObject>
#include <osg/Vec2>
//#include "SlitPotential.h"
#include <osg/ref_ptr>
using osg::ref_ptr;
#include "BQTDefine.h"
#include "MathUtils.h"

namespace ews {
    namespace app {
        namespace model {
            using osg::Vec2;
            using ews::util::imposeBounds;
            using ews::util::scale;
                        
            class BarrierSet;
            
            /**
             * Contains the business logic for barrier objects to be drawn on the screen, as well as
             * a reference to the SlitPotential necessary for physics calculations.
             */
            class Barrier :  public QObject  {
                Q_OBJECT
                Q_ENUMS(NumSlits)
                Q_PROPERTY(Vec2 start READ getStart WRITE setStart)
                Q_PROPERTY(Vec2 end READ getEnd WRITE setEnd)
                Q_PROPERTY(NumSlits numSlits READ getNumSlits WRITE setNumSlits)
                Q_PROPERTY(Uint slitWidth READ getSlitWidth WRITE setSlitWidth)
                Q_PROPERTY(Uint slitSeparation READ getSlitSeparation WRITE setSlitSeparation)
                
            public:
                /**
                 * Possible slit configurations.
                 */
                enum NumSlits { ZERO_SLITS, ONE_SLIT, TWO_SLITS };
                
                /**
                 * Default constructor.
                 * @param parent BarrierSet this Barrier belongs to.
                 */
                explicit Barrier(BarrierSet* parent/*CRUFT? = 0*/);
                
                virtual ~Barrier();

                
                /** Get the owner of this. */
                BarrierSet* getBarrierSet();

                /**
                 * Gets enabled state of barrier. 
                 * @return true if active, false if inactive and invisible.
                 */
                bool isEnabled() const {
                    return _enabled;
                }
                
                /**
                 * Number of slits in barrer {0,1,2}
                 */
                NumSlits getNumSlits() const {
                    return _numSlits;
                }
                
                /**
                 * Size of the slit when one or more slits, where 0 is the smallest possible
                 * width for this configuration, and 100 is the maximum possible width for this 
                 * configuration. I.e., for one slit, 100 corresponds to a width just short of the
                 * wall length, and for two slits, 100 corresponds to a width just short of half the
                 * wall length.
                 */
                Uint getSlitWidth() const {
                    return _slitWidth;
                }
                
                /**
                 * Calculates the slit width as a percentage of the barrier length.
                 */
                Real calculateSlitWidthAlpha() const {
                    // Two slits need to allow for a tiny bit of wall on either end and one in the middle
                    Real maxVal = (_numSlits == TWO_SLITS) ? (.5f - 3 * minAlpha())
                                : (1.f - 2 * minAlpha());
                    return scale(minAlpha(), static_cast<Real>(_slitWidth) / 100.f, maxVal);
                }
                
                /**
                 * Distance between slits when more than one slit, where 0 is the smallest possible
                 * separation, and 100 is the maximum possible separation. I.e., 100 corresponds to
                 * a slit separation just short of the wall length less two slit widths.
                 */
                Uint getSlitSeparation() const {
                    return _slitSeparation;
                }
                
                /**
                 * Calculates the slit separation as a percentage of the barrier length.
                 */
                Real calculateSlitSeparationAlpha() const {
                    // Two slits need to allow for a tiny bit of wall on either end and one in the middle
                    Real maxVal = 1.f - 2 * calculateSlitWidthAlpha() - 3 * minAlpha();
                    return scale(minAlpha(), static_cast<Real>(_slitSeparation) / 100.f, maxVal);
                }
                
                /**
                 * Distance between start and end points. 
                 * @return |end - start|
                 */
                Real length() const {
                    return (_end - _start).length();
                }
                
                /**
                 * Fixed wall thickness.
                 */
                static Real width() {
                    return 1.0;
                }
                
                /**
                 * Get the barrier start point (2-D water surface coordinates).
                 */
                const osg::Vec2& getStart() const {
                    return _start;
                }
                
                /**
                 * Get the barrier end point (2-D water surface coordinates).
                 */
                const osg::Vec2& getEnd() const {
                    return _end;
                }


                
                /**
                 * Set all the configurable parameters at once, resulting in a single
                 * dataChanged() signal and a recomputed potential.
                 * @param start start point in 2-D water suface coordinates
                 * @param end end point in 2-D water suface coordinates
                 * @param numSlits number of slits
                 * @param slitWidth size of slit(s)
                 * @param slitSeparation distance between slit edges when numSlits > ONE
                 */
                void set(Vec2 start, Vec2 end, NumSlits numSlits, Uint slitWidth, Uint slitSeparation) {
                    _start = start;
                    _end = end;
                    _numSlits = numSlits;
                    _slitWidth = slitWidth;
                    _slitSeparation = slitSeparation;
                    emit dataChanged();
                }
                
            public slots:
                
                /**
                 * Change the enabled state of this.
                 * @param enabled True to make barrier visible and active
                 * in simulation, false to hide and remove from simulation.
                 * @signals dataChanged()
                 */
                void setEnabled(bool enabled) {
                    _enabled = enabled;
                    emit dataChanged();
                }
                
                /**
                 * Set the number of slits for this Barrier.
                 */
                void setNumSlits(NumSlits num) {
                    _numSlits = num;
                    emit dataChanged();
                }
                
                /**
                 * Set the slit width from 0 to 100, where 0 is the smallest possible width for this
                 * configuration, and 100 is the maximum possible width for this configuration.
                 * I.e., for one slit, 100 corresponds to a width just short of the wall length, and
                 * for two slits, 100 corresponds to a width just short of half the wall length.                 
                 */
                void setSlitWidth(Uint slitWidth) {
                    _slitWidth = slitWidth;
                    emit dataChanged();
                }
                
                /**
                 * Set the distance between slits when more than one slit, where 0 is the smallest
                 * possible separation, and 100 is the maximum possible separation. I.e., 100
                 * corresponds to a slit separation just short of the wall length less two slit
                 * widths.
                 */                
                void setSlitSeparation(Uint slitSeparation) {
                    _slitSeparation = slitSeparation;
                    emit dataChanged();
                }
                
                /**
                 * Set the starting point for this barrier. There is no practical difference
                 * between the starting point and ending point of a barrier.
                 */
                void setStart(const osg::Vec2& start) {
                    _start = start;
                    emit dataChanged();
                }
                
                /**
                 * Set the ending point for this barrier. There is no practical difference
                 * between the starting point and ending point of a barrier.
                 */
                void setEnd(const osg::Vec2& end) {
                    _end = end;
                    emit dataChanged();
                }

            signals:
                void dataChanged();
                //void potentialChanged();
            
            private slots:
              //  void generatePotential();
                
            private:
                Q_DISABLE_COPY(Barrier)
                /**
                 * Returns the smallest possible alpha for this barrier. Larger walls can have a
                 * smaller alpha.
                 */
                Real minAlpha() const {
                    // Basically, the minimum alpha corresponds to approximately a single "unit"
                    return 1.f / (_end - _start).length();
                }
                bool _enabled;
                NumSlits _numSlits;
                Uint _slitWidth;
                Uint _slitSeparation;
                osg::Vec2 _start;
                osg::Vec2 _end;
            };
        }
    }
}


#endif // __BARRIER_H

