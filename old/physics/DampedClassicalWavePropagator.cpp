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

#include "DampedClassicalWavePropagator.h"
#include <cmath>

namespace ews {
    namespace physics {
        const bool DAMP_EDGES = true;
        
        DampedClassicalWavePropagator::DampedClassicalWavePropagator(Potential* p,
                                                                     unsigned int width, unsigned int length,
                                                                     unsigned int dampX, unsigned int dampY)
        : _potential(new PaddedPotential(p, dampX, dampY)),  _largeLattice(width + 2 * dampX, length + 2 * dampY),
        _priorLattice(width + 2 * dampX, length + 2 * dampY),
        _priorPriorLattice(width + 2 * dampX, length + 2 * dampY), _dampX(dampX), _dampY(dampY) {
            /* do nothing */
        }
        void DampedClassicalWavePropagator::setBoundaryCondition(unsigned int x, unsigned int y,
                                                                 LatticeVal value) {
            _priorLattice.setValue(x + _dampX, y + _dampY, value );
            _priorPriorLattice.setValue(x + _dampX, y + _dampY, value );
        }
        void DampedClassicalWavePropagator::setPotential(Potential* p) {
            _potential = new PaddedPotential(p, _dampX, _dampY);
        }
        void DampedClassicalWavePropagator::propagate(Lattice& lattice) {            
            unsigned int width = _largeLattice.getWidth() - 1;
            unsigned int length = _largeLattice.getLength() - 1;
            _largeLattice = _priorLattice; // At this point, largeLattice and priorLattice should already be the same
            for (unsigned int i = 1; i < width; i++) {
                LatticeVal* priorRow = _priorLattice.getRow(i - 1);
                LatticeVal* row = _priorLattice.getRow(i);
                LatticeVal* nextRow = _priorLattice.getRow(i + 1);
                LatticeVal* oldRow = _priorPriorLattice.getRow(i);
                LatticeVal* newRow = _largeLattice.getRow(i);
                for (unsigned int j = 1; j < length; j++) {
                    if (_potential->getPotential(i, j) != 0) {
                        newRow[j] = 0.0f;
                    }
                    else {
                        const LatticeVal neigh = (priorRow[j] + row[j-1] + row[j+1] + nextRow[j]) * NEIGHBOR_PROPAGATE +
                        (priorRow[j-1] + priorRow[j+1] + nextRow[j-1] + nextRow[j+1]) * DIAG_PROPAGATE;
                        newRow[j] = MULT_FACTOR * (row[j] * SELF_PROPAGATE - oldRow[j] * MEMORY_TERM + neigh);
                    }
                }
            }
            if (DAMP_EDGES) {
                dampHorizontal(0, 1);
                dampHorizontal(length, -1);
                dampVertical(0, 1);
                dampVertical(width, -1);
            }
                
            _priorPriorLattice = _priorLattice;
            _priorLattice = _largeLattice;            
            
            if (DAMP_EDGES) {
                dampScale();
            }
            for (unsigned int i = 0; i < lattice.getWidth(); i++) {
                LatticeVal* row = _largeLattice.getRow(i + _dampX);
                LatticeVal* retRow = lattice.getRow(i);
                for (unsigned int j = 0; j < lattice.getLength(); j++) {
                    retRow[j] = row[j + _dampY];
                }
            }
        }
        void DampedClassicalWavePropagator::scale(LatticeVal scaleVal) {
            _priorLattice.scale(scaleVal);
            _priorPriorLattice.scale(scaleVal);
        }
        void DampedClassicalWavePropagator::setSize(unsigned int width, unsigned int length) {
            const unsigned int requiredWidth = width + 2 * _dampX;
            const unsigned int requiredLength = length + 2 * _dampY;
            if (requiredWidth != _largeLattice.getWidth() || requiredLength != _largeLattice.getLength()) {
                _largeLattice.setSize(requiredWidth, requiredLength);
                _priorLattice.setSize(requiredWidth, requiredLength);
                _priorPriorLattice.setSize(requiredWidth, requiredLength);
            }
        }
        void DampedClassicalWavePropagator::clear() {
            _priorLattice.clear();
            _priorPriorLattice.clear();
        }
        void DampedClassicalWavePropagator::dampHorizontal(unsigned int y, int delta) {
            for (unsigned int i = 0; i < _largeLattice.getWidth(); i++) {
                _largeLattice.setValue(i, y, _priorPriorLattice.getValue(i, y + delta));
            }
        }
        void DampedClassicalWavePropagator::dampHorizontal(unsigned int y, int delta, unsigned int numDampPts) {
            for (unsigned int i = 0; i < _largeLattice.getWidth(); i++) {
                for (unsigned int step = 0; step < numDampPts; step++) {
                    const unsigned int distFromDampBoundary = numDampPts - step;
                    const LatticeVal damp = getDamp(distFromDampBoundary);
                    unsigned int j = y + step * delta;
                    _largeLattice.scaleLocation(i, j, damp);
                    _priorLattice.scaleLocation(i, j, damp);
                }
            }
        }
        void DampedClassicalWavePropagator::dampVertical(unsigned int x, int delta) {
            for (unsigned int j = 0; j < _largeLattice.getLength(); j++) {
                _largeLattice.setValue(x, j, _priorPriorLattice.getValue(x + delta, j));
            }
        }
        void DampedClassicalWavePropagator::dampVertical(unsigned int x, int delta, unsigned int numDampPts) {
            for (unsigned int j = 0; j < _largeLattice.getLength(); j++ ) {
                for (unsigned int step = 0; step < numDampPts; step++) {
                    const unsigned int distFromDampBoundary = numDampPts - step;
                    const LatticeVal damp = getDamp(distFromDampBoundary);
                    unsigned int i = x + step * delta;
                    _largeLattice.scaleLocation(i, j, damp);
                    _priorLattice.scaleLocation(i, j, damp);
                }
            }
        }
        void DampedClassicalWavePropagator::dampScale() {
            dampVertical(0, 1, _dampY / 2);
            dampVertical(_largeLattice.getWidth() - 1, -1, _dampY / 2);
            dampHorizontal(0, 1, _dampX / 2);
            dampHorizontal(_largeLattice.getLength() - 1, -1, _dampX / 2);
        }
        
        LatticeVal DampedClassicalWavePropagator::getDamp(unsigned int depthInDampRegion) {
            return (1 - depthInDampRegion * 0.0001);
        }
    }
}
