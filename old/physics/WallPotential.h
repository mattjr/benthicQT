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

#ifndef __WALLPOTENTIAL_H
#define __WALLPOTENTIAL_H

#include "Potential.h"
#include "Line2d.h"
#include <osg/Vec2>
#include <cmath>
using std::sqrt;
#include "EWSDefine.h"

namespace ews {
    namespace physics {
        const Real DEFAULT_HALF_THICKNESS_SQ = 2 * 2;
        const Real DEFAULT_POTENTIAL = 100.0;
        using osg::Vec2;

        /**
         * @ingroup Physics
         * Potential describing a wall or mirror
         */        
        class WallPotential : public Potential {
        public:
            /**
             * Default constructor, with both points at origin (0, 0)
             */
            WallPotential():
            _lineSegment(Vec2(0.0, 0.0), Vec2(0.0, 0.0)), _halfThicknessSq(DEFAULT_HALF_THICKNESS_SQ) {
                /* do nothing */
            }
            /**
             * Constructor for a wall with end points p1, p2 and thickness 3.
             * @param p1 One point of the wall
             * @param p2 The other point of the wall
             */
            WallPotential(const Vec2& p1, const Vec2& p2):
            _lineSegment(p1, p2), _halfThicknessSq(DEFAULT_HALF_THICKNESS_SQ) {
                /* do nothing */
            }
            /**
             * Constructor for a wall with end points p1, p2 and thickness 3.
             * @param p1 One point of the wall
             * @param p2 The other point of the wall
             * @param thickness How thick the wall is
             */
            WallPotential(const Vec2& p1, const Vec2& p2, Real thickness):
            _lineSegment(p1, p2), _halfThicknessSq(thickness * thickness / 4.f) {
                /* do nothing */
            }
            /**
             * Virtual destructor
             */
            virtual ~WallPotential() { /* do nothing */}
            /**
             * Returns a potential of 100 if and only if x, y lie within the wall.
             * @param x X location to check for wall
             * @param y Y location to check for wall
             * @return 100 if x, y lie within wall, 0 otherwise
             */
            virtual Real getPotential(unsigned int x, unsigned int y) const;
            /**
             * Returns one of the wall's end points
             * @Return wall end point
             */
            const Vec2& getSrcPoint() const { return _lineSegment.getStart(); }
            /**
             * Changes one of the wall's end points
             * @param p New end point to use
             */
            void setSrcPoint(const Vec2& p) {
                _lineSegment.setStart(p);
            }
            /**
             * Returns the wall's other end point
             * @Return other wall end point
             */
            const Vec2& getDstPoint() const { return _lineSegment.getEnd(); }
            /**
             * Changes the wall's other end point
             * @param p New end point to use
             */
            void setDstPoint(const Vec2& p) {
                _lineSegment.setEnd(p);
            }
            /**
             * Returns the thickness of the wall.
             * @return Wall thickness
             */
            Real getThickness() const { return sqrt(_halfThicknessSq * 4.f); }
            /**
             * Returns the length of the wall (i.e., the distance between its two end points.
             */
            Real length() const { return _lineSegment.length(); }
            /**
             * Sets the thickness of the wall.
             * @param thickness New thickness for the wall.
             */
            void setThickness(Real thickness) { _halfThicknessSq = thickness * thickness / 4.f; }
        protected:
            /**
             * Calculates an alpha value from projecting an (x, y) coordinate on to the associated wall.
             * Values outside of (0, 1) do not "hit" the wall.
             * @param x X-value of coordinate
             * @param y Y-value of coordinate
             * @return Calculated alpha value
             */
            Real alpha(unsigned int x, unsigned int y) const;
            /**
             * Calculates an interpolated point a fraction of the distance from the beginning to the
             * end of the associated wall.
             * @param alphaVal fractional distance to calculate for
             * @return Interpolated point
             */
            inline Vec2 interpolate(Real alphaVal) const {
                Vec2 interpolatedPoint;
                _lineSegment.interpolate(alphaVal, interpolatedPoint);
                return interpolatedPoint;
            }
        private:
            WallPotential(const WallPotential&) {} // Not allowed
            WallPotential& operator=(const WallPotential& l) { return *this; } // Not allowed
            Line2d _lineSegment;
            Real _halfThicknessSq;
        };
    }
}

#endif // __WALLPOTENTIAL_H
