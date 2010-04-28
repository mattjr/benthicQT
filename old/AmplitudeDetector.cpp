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

#include "AmplitudeDetector.h"
#include "SimulationState.h"
#include "WaveMedium.h"
#include "WaterBoundaryDragConstraint.h"
#include <osg/ShapeDrawable>

namespace ews {
    namespace  app {
        namespace drawable {
            using namespace osg;
            /** How tall the detector is above surface. */
            const Real DETECTOR_VISIBLE_HEIGHT = 10;
            /** Where knob sits relative to to the geometry origin. */
            const Vec3 KNOB_OFFSET(0, 0, DETECTOR_VISIBLE_HEIGHT+2);
            /** Radius of detector geometry. */
            const Real DETECTOR_RADIUS = 1.5;
            
            /**
             * Private class responsible for updating sampler when it's moved.
             */
            class SampleUpdater :  public NodeCallback  {
            public:
                SampleUpdater(Knob& dragger) : _dragger(dragger) {
                }
                
                /** Callback method called each render loop. */
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    AmplitudeDetector* geom = dynamic_cast<AmplitudeDetector*>(node);
                    if(geom) {
                        
                        PointSampler& s = geom->getDataModel();
                        if(_dragger.isDirty()) {
                            s.setPosition(_dragger.currXYLocation());
                            _dragger.setDirty(false);
                        }
                        
//                        const FrameStamp* fs = nv->getFrameStamp();
//                        int fn = fs->getFrameNumber();
                        s.sample();
                    }
                    else {
                        notify(osg::WARN) << "AmplitudeDetector didn't receieve the right geometry type";
                        return;
                    }
                    
                    traverse(node,nv);
                }
            private:
                Knob& _dragger;
                
            };
                
            AmplitudeDetector::AmplitudeDetector(PointSampler& sampler) 
            : DrawableQtAdapter(), _sampler(sampler), _dragger(new Knob(KNOB_OFFSET, DETECTOR_RADIUS + 2, true)), 
            _geom(new PositionAttitudeTransform), _switch(new Switch)  {
                using ews::app::model::SimulationState;
                
                _switch->setNewChildDefaultValue(true);
                addChild(_switch.get());
                _switch->addChild(_dragger.get());
                _switch->addChild(_geom.get());
                createGeom();
                
                // Synchronize starting position.
                setPosition(_sampler.getPosition());
                _dragger->setPosition(_geom->getPosition());
                
                
                // Limit movement to within the water area.
                // TODO: Code duplicated in FaucetGeom; find abstraction.
                ews::app::model::WaveMedium& waves = _sampler.getSimulationState()->getWaveMedium();
                BoundingBox waterArea(0, 0, 0, waves.getWidth(), waves.getLength(), 0);
                ref_ptr<Constraint> constraint = new WaterBoundaryDragConstraint(*this, waterArea);
                _dragger->setConstraint(*(constraint.get()));
                
                // Attach render-loop callback to update samples.
                setUpdateCallback(new SampleUpdater(*_dragger));
                
                QObject::connect(&_sampler, SIGNAL(enabledChanged(bool)), this, SLOT(setEnabled(bool)));
                QObject::connect(&_sampler, SIGNAL(positionChanged(osg::Vec2)), this, SLOT(setPosition(const osg::Vec2&)));
                
                setEnabled(_sampler.isEnabled());
            }
            
            AmplitudeDetector::~AmplitudeDetector() {
            }
            
            void AmplitudeDetector::setEnabled(bool enabled) {
                if(enabled) {
                    _switch->setAllChildrenOn();
                }
                else {
                    _switch->setAllChildrenOff();
                }
            }
            
            void AmplitudeDetector::setPosition(const osg::Vec2& pos) {
                Vec3 newPos(pos.x(), pos.y(), 0);
                _geom->setPosition(newPos);
            }
            
            void AmplitudeDetector::createGeom() {
                // Create geometric representation 
                ref_ptr<PositionAttitudeTransform> orient = new PositionAttitudeTransform;
                _geom->addChild(orient.get());
                
                ref_ptr<Geode> geode = new Geode;
                orient->addChild(geode.get());
                const int surfOffset = 7;
                ref_ptr<Cone> c = new Cone(Vec3(0, 0, surfOffset), DETECTOR_RADIUS, DETECTOR_VISIBLE_HEIGHT + surfOffset);
                c->setRotation(Quat(M_PI, Vec3(1, 0, 0)));
                ref_ptr<ShapeDrawable> d = new ShapeDrawable(c.get());
                
                const QColor& color = _sampler.getColor();
                d->setColor(Vec4(color.redF(), color.greenF(), color.blueF(), 1.f));
                geode->addDrawable(d.get());
                
            }
        }
    }
}