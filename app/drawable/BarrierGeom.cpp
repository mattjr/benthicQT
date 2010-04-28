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

#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/CullFace>

#include <osgManipulator/Translate2DDragger>

#include "BarrierGeom.h"
#include "PotentialUpdater.h"
#include "BQTDebug.h"
#include "BarrierSet.h"
#include "SimulationState.h"
#include "WaterBoundaryDragConstraint.h"
namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            using namespace app::model;
            
            /** How far the barriers can go outside of the water region. */
            const Uint ALLOWABLE_OUT_OF_BOUNDS = 5;
            /** How tall the barriers appear. */
            const Real VISIBLE_BARRIER_HEIGHT = 10;
            /** How wide the barriers are. */
            const float VISIBLE_BARRIER_WIDTH = 6.f;
            /** How opaque the barriers are. */
            const float BARRIER_OPACITY = 1.f;
            /** Where starting knob is relative to to the starting point. */
            const Vec3 START_KNOB_OFFSET(0, 0, 7);
            /** Where ending knob is relative to to the ending point. */
            const Vec3 END_KNOB_OFFSET(0, 0, 7);
            /** The color of the barrier. */
            const Vec4 BARRIER_COLOR(1.f, 0.f, 0.f, BARRIER_OPACITY);
            /** Which plane the barrier is constrained to. */
            const Plane BARRIER_PLANE(osg::X_AXIS, 0);

            
            /** Primary constructor. */
            BarrierGeom::BarrierGeom(Barrier& dataModel) 
            : DrawableQtAdapter(), _dataModel(dataModel), _switch(new Switch),
            _barrierGeom(new PositionAttitudeTransform), 
            _startKnob(new Knob(START_KNOB_OFFSET)), _endKnob(new Knob(END_KNOB_OFFSET)) {
                
                setColor(BARRIER_COLOR); 
                
                _switch->setNewChildDefaultValue(true);
                addChild(_switch.get());
                _switch->addChild(_barrierGeom.get());
               // WaveMedium& waves = _dataModel.getBarrierSet()->getSimulationState()->getWaveMedium();
                const Real wiggle = ALLOWABLE_OUT_OF_BOUNDS;
                BoundingBox validArea(-wiggle, -wiggle, 0,
                                      WIDTH_WATER + wiggle, HEIGHT_WATER + wiggle, 0);
                ref_ptr<Constraint> constraint = new WaterBoundaryDragConstraint(*this, validArea);
                
                // Knob locations are in gobal coordinates.
                _startKnob->setPosition(Vec3(_dataModel.getStart(), 0));
                _startKnob->setConstraint(*(constraint.get()));
                _endKnob->setPosition(Vec3(_dataModel.getEnd(), 0));
                _endKnob->setConstraint(*(constraint.get()));
                
                _startKnob->setName("startBarrierKnob");
                _switch->addChild(_startKnob.get());
                _endKnob->setName("endBarrierKnob");
                _switch->addChild(_endKnob.get());
                
                updateGeom();
                
                // Callback to detect when we've been moved
                // and update the databmodel.
                setUpdateCallback(new PotentialUpdater);
                
                respondToSignals(true);
            }
            
            
            BarrierGeom::~BarrierGeom() {
            }
            
            void BarrierGeom::respondToSignals(bool respond) {
                if(respond) {
                    connect(&_dataModel, SIGNAL(dataChanged()), SLOT(updateGeom()));//, Qt::UniqueConnection);
                }
                else {
                    _dataModel.disconnect(this);
                }
            }
            
            void BarrierGeom::setColor(const Vec4& color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT, Vec4(0.f, 0.f, 0.f, BARRIER_OPACITY));
                mat->setAmbient(Material::FRONT, color);
                mat->setShininess(Material::FRONT, 95.f);
                state->setAttribute(mat.get());
                if(BARRIER_OPACITY < 1) {
                    state->setMode(GL_BLEND, StateAttribute::ON); // Activate blending (for transparency)
                }
//                ref_ptr<CullFace> cf = new CullFace(CullFace::BACK);
//                state->setAttribute(cf.get());
                
            }
            
            void BarrierGeom::addBox(const ref_ptr<Geode>& geode, Real boxCenter, Real boxLength) {
                ref_ptr<ShapeDrawable> d = new ShapeDrawable();
                const Real boxWidth = _dataModel.width();
                ref_ptr<Shape> s = new Box(Vec3(boxCenter, 0.f, 0.f),
                                                     boxLength, boxWidth,
                                                     VISIBLE_BARRIER_HEIGHT);
                ref_ptr<TessellationHints> hints = new TessellationHints();
                d->setShape(s.get());
                geode->addDrawable(d.get());
            }
            
            void BarrierGeom::setEnabled(bool enabled) {
                if(enabled) {
                    _switch->setAllChildrenOn();
                }
                else {
                    _switch->setAllChildrenOff();
                }
            }
            
            void BarrierGeom::updateGeom() {
                _barrierGeom->removeChildren(0, _barrierGeom->getNumChildren());
                const Vec2& start = _dataModel.getStart();
                _barrierGeom->setPosition(Vec3d(start.x(), start.y(), 0.f));
                const Vec2& end = _dataModel.getEnd();
                const Vec2 dir = (end - start);
                const Real barrierLength = _dataModel.length();
                _barrierGeom->setScale(Vec3f(barrierLength, 1.f, 1.f));
                const Real angle = atan2(dir.y(), dir.x());
                _barrierGeom->setAttitude(Quat(angle, Vec3f(0.f, 0.f, 1.f)));
                
                // Create geometric representation
                ref_ptr<Geode> geom = new Geode;
                _barrierGeom->addChild(geom);
                if (_dataModel.getNumSlits() == Barrier::ZERO_SLITS) {
                    addBox(geom, 0.5f, 1.f);
                }
                else {
                    const Real slitAlpha = _dataModel.calculateSlitWidthAlpha();
                    if (_dataModel.getNumSlits() == Barrier::ONE_SLIT) {
                        const Real boxLength = 0.5f - (slitAlpha / 2);
                        addBox(geom, boxLength / 2, boxLength);
                        addBox(geom, 1 - boxLength / 2, boxLength);
                    }
                    else {
                        // Assumes that barrierLength > slitSeparation + 2 * slitWidth
                        const Real separationAlpha = _dataModel.calculateSlitSeparationAlpha();
                        // Box length for boxes at either end of barrier
                        const Real boxLength = 0.5f - slitAlpha - (separationAlpha / 2);
                        addBox(geom, boxLength / 2, boxLength);
                        addBox(geom, 1 - boxLength / 2, boxLength);
                        // Box between the two slits
                        addBox(geom, 0.5f, separationAlpha);
                    }
                }
                
                setEnabled(_dataModel.isEnabled());
            }

            void BarrierGeom::checkKnobs() {
                if(_startKnob->isDirty()) {
                    Vec2 kStart = _startKnob->currXYLocation();
                    _dataModel.setStart(kStart);
                    _startKnob->setDirty(false);
                }
                if(_endKnob->isDirty()) {
                    Vec2 kend = _endKnob->currXYLocation();
                    _dataModel.setEnd(kend);
                    _endKnob->setDirty(false);
                }
            }
        }
    }
}
