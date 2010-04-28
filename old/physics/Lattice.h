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

#ifndef __LATTICE_H
#define __LATTICE_H

#include <vector>
using std::vector;
// Needed if asserts are turned back on
//#include <cassert>
#include <cstring> // Needed for memcpy
#include <cmath>

namespace ews {
    namespace physics {
        const unsigned int DEFAULT_WINDOW_WIDTH = 2;

        /**
         * Value type to use for operations that will be rendered by openGL.
         */
        typedef float LatticeVal;
        
        
        /**
         * @ingroup Physics
         * Structure for holding the field state
         */        
        class Lattice {
            
        public:
            /**
             * Default constructor
             */
            Lattice(): _amplitudeData(NULL), _width(0), _length(0) {
            }
            /**
             * Creates a lattice of size width x length, initialized to 0
             * @param width Width of the lattice
             * @param length Length of the lattice
             */
            Lattice(unsigned int width, unsigned int length): _amplitudeData(NULL), _width(0), _length(0) {
                setSize(width, length);
            }
            
            /**
             * Destructor
             */
            virtual ~Lattice() {
                if(_amplitudeData) {
                    delete[] _amplitudeData;
                }
            }
            /**
             * Determines whether x and y are within the purview of this lattice
             * @param x X location to check
             * @param y Y location to check
             * @return Whether (x, y) are in this lattice
             */
            bool containsLocation(unsigned int x, unsigned int y) const {
                return (0 <= x) && (x < getWidth()) && (0 <= y) && (y < getLength());
            }
            
            /**
             * Computes the average value of the lattice at x, y with an averaging window of width windowWidth.
             * @param x X location to compute the average for
             * @param y Y location to compute the average for
             * @param windowWidth Width of the averaging window (0 is a single cell)
             * @return Average value for the specified window.
             */
            LatticeVal computeAverageValue(unsigned int x, unsigned int y, unsigned int windowWidth) const;
            
            /**
             * Applies a normalization transformation on the average 
             * value of the lattice at x, y with an averaging window of width windowWidth.
             * @param x X location to compute the normalized average for
             * @param y Y location to compute the normalized average for
             * @param windowWidth Width of the averaging window (0 is a single cell)
             * @return Average value for the specified window.
             * @note This is experimental at the moment.
             */
            inline LatticeVal computeNormalizedValue(unsigned int x, unsigned int y, unsigned int windowWidth) const {
                static const LatticeVal sigma = 2;
                static const LatticeVal twoSigmaSqr = 2 * sigma * sigma;
                
                LatticeVal retval = computeAverageValue(x, y, windowWidth);
                retval = retval * exp(-retval*retval/twoSigmaSqr);
                return retval;
            }
            
            /**
             * Gets the width of the lattice.
             * @return Width of the lattice
             */
            inline unsigned int getWidth() const {
                return _width;
            }
            
            /**
             * Gets the length of the lattice.
             * @return Length of the lattice
             */
            inline unsigned int getLength() const {
                return _length;
            }
            
            /**
             * Get the total number of elements in the Lattice.
             */
            inline unsigned int getSize() const {
                return _length * _width;
            }
            
            /**
             * Sets the width and length of the lattice, and resets all amplitudes to zero.
             * @param width Width of the lattice
             * @param length Length of the lattice
             */
            void setSize(unsigned int width, unsigned int length) {
                _width = width;
                _length = length;
                
                if(_amplitudeData){
                    delete[] _amplitudeData;
                }
                
                unsigned int len = getSize();
                _amplitudeData = new LatticeVal[len];
                while(len--) {
                    _amplitudeData[len] = 0;
                }
            }
            
            /**
             * Gets the amplitude of the lattice at location x,y as a writable value.
             * @param x X location to get the amplitude for
             * @param y Y location to get the amplitude for
             */
            inline LatticeVal& getValue(unsigned int x, unsigned int y) {
//                assert(x < getWidth() && y < getLength());
                return  _amplitudeData[x*getLength() + y];
            }
            
            /**
             * Gets an entire row of amplitude information as read/writeable values.
             * @param x Row to retrieve
             */
            inline LatticeVal* getRow(unsigned int x) {
                return &_amplitudeData[x * getLength()];
            }
            
            /**
             * Gets the amplitude of the lattice at location x,y
             * @param x X location to get the amplitude for
             * @param y Y location to get the amplitude for
             */
            inline LatticeVal getValue(unsigned int x, unsigned int y) const {
//                assert(x < getWidth() && y < getLength());
                return  _amplitudeData[x*getLength() + y];
            }
            
            /**
             * Sets the amplitude of the lattice at location x,y to value
             * @param x X location to set the amplitude at
             * @param y Y location to set the amplitude at
             * @param value New value of the amplitude
             */
            inline void setValue(unsigned int x, unsigned int y, LatticeVal value) {
                getValue(x, y) = value;
            }
            /**
             * Scales all amplitude values by scaleVal
             * @param scaleVal amount to scale by
             */
            void scale(LatticeVal scaleVal);
            
            /**
             * Scales amplitude value at x, y by scaleVal
             * @param x X location to scale the amplitude for
             * @param y Y location to scale the amplitude for
             * @param scaleVal amount to scale by
             */
            inline void scaleLocation(unsigned int x, unsigned int y, LatticeVal scaleVal) {
                getValue(x, y) *= scaleVal;
            }
            
            /**
             * Sets all amplitude values to zero
             */
            void clear() {
                setSize(_width, _length);
            }
            
            /**
             * Assignment operator; copy another lattice into this one.
             */
            Lattice& operator=(const Lattice& l) {
                // Set size makes deletes current memory
                // and allocates enough for the new array
                if(getWidth() != l.getWidth() || getLength() != l.getLength()) { 
                    setSize(l.getWidth(), l.getLength());
                }
                memcpy(_amplitudeData, l._amplitudeData, getSize() * sizeof(LatticeVal));
                return *this;
            }

                           
        private:
            Lattice(const Lattice&) {} // Not allowed
            LatticeVal* _amplitudeData;
            unsigned int _width;
            unsigned int _length;
        };
    }
}

#endif // __LATTICE_H
