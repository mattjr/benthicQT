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

#include <osg/Geode>
#include <osg/Drawable>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/NodeCallback>
#include "FaucetGeom.h"
#include "Oscillator.h"
#include "WaveMedium.h"
#include "Teapot.h"
#include "SimulationState.h"
#include <QtGlobal>
#include "EWSDebug.h"
#include "WaterBoundaryDragConstraint.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using ews::physics::Oscillator;
            using ews::app::model::WaveMedium;
            
            /**
             * Private class responsible for updating the oscillator simulation
             */
            class FaucetUpdater :  public NodeCallback  {
                
            private:
                void updateOscillator(FaucetGeom* geom, NodeVisitor* nv) {
                    
                    if(!geom->getDataModel().isPaused()) {
                        const FrameStamp* fs = nv->getFrameStamp();
                        Real value = fs->getSimulationTime();
                        
                        Oscillator& o = geom->getDataModel().getOscillator();
                        o.updateTimeAndOscillator(value);
                    }
                }
                
                void detectAndHandleMove(FaucetGeom* geom, NodeVisitor* nv) {
                    if (_dragger.isDirty()) {
                        geom->getDataModel().setPosition(_dragger.currXYLocation());
                        _dragger.setDirty(false);
                    }
                }
                
            public:
                FaucetUpdater(Knob& dragger) : _dragger(dragger) {
                }
                
                /** Callback method. */
                virtual void operator()(Node* node, NodeVisitor* nv) { 
                    FaucetGeom* geom = dynamic_cast<FaucetGeom*>(node);
                    if (geom != NULL) {
                        detectAndHandleMove(geom, nv);
                        updateOscillator(geom, nv);
                    }
                    else {
                        notify(osg::WARN) << "OscillatorUpdater didn't receieve the right geometry type";
                        return;
                    }
                    
                    traverse(node,nv);
                }
            private:                
                Knob& _dragger;
            };
            
            
            /** Standard ctor. */
            FaucetGeom::FaucetGeom(DripSource& dataModel) 
            : DrawableQtAdapter(), _dataModel(dataModel), 
            _dragger(new Knob(Vec3(-20,0,5), 12, false)), _geom(new PositionAttitudeTransform) {
                
                addChild(_dragger.get());
                
                addChild(_geom.get());
                createGeom();
                setPosition(_dataModel.getPosition());
                
                // Make sure the dragger starts off at the same location as geom
                _dragger->setPosition(_geom->getPosition());
                
                WaveMedium& waves = _dataModel.getSimulationState()->getWaveMedium();
                BoundingBox waterArea(0, 0, 0, waves.getWidth(), waves.getLength(), 0);
                
                ref_ptr<Constraint> constraint = new WaterBoundaryDragConstraint(*this, waterArea);
                _dragger->setConstraint(*(constraint.get()));
                
                setColor(_dataModel.getColor()); 

                // Attach render-loop callback to update oscillator.
                setUpdateCallback(new FaucetUpdater(*_dragger));
                
                QObject::connect(&_dataModel, SIGNAL(drip(int)), this, SLOT(drip()));
                QObject::connect(&_dataModel, SIGNAL(enabledChanged(bool)), this, SLOT(setEnabled(bool)));
                QObject::connect(&_dataModel, SIGNAL(positionChanged(osg::Vec2)), this, SLOT(setPosition(const osg::Vec2&)));
                QObject::connect(&_dataModel, SIGNAL(colorChanged(osg::Vec4)), this, SLOT(setColor(const osg::Vec4&)));
                
                
                setEnabled(_dataModel.isEnabled());
            }
            
            FaucetGeom::~FaucetGeom() {
            }
            
            
            void FaucetGeom::createGeom() {
                // Create geometric representation

                ref_ptr<PositionAttitudeTransform> facetOrient = new PositionAttitudeTransform;
                _geom->addChild(facetOrient.get());
                
                ref_ptr<Geode> geode = new Geode;
                facetOrient->addChild(geode.get());
                
                ref_ptr<osg::Drawable> d = new Teapot;
                geode->addDrawable(d.get());

                // Orient the teapot so it looks like the source of drips
                // is at the spout.
                facetOrient->setAttitude(Quat(osg::DegreesToRadians(50.0f), Vec3f(0, 1, 0)));
                facetOrient->setPosition(Vec3(-24, 0, 0));
                facetOrient->setScale(Vec3(5, 5, 5));

            }
            
            void FaucetGeom::setColor(osg::Vec4 color) {
                ref_ptr<StateSet> state = getOrCreateStateSet(); 
                ref_ptr<Material> mat = new osg::Material; 
                mat->setDiffuse(Material::FRONT, color);
                mat->setSpecular(Material::FRONT,
                                 Vec4( 0.8f, 0.8f, 0.8f, 0.8f ) ); 
                mat->setShininess(Material::FRONT, 96.f ); 
                mat->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
                state->setAttribute( mat.get() );

#if defined(GL_MULTISAMPLE_ARB)
                // Antialias faucet.
                state->setMode(GL_MULTISAMPLE_ARB, StateAttribute::ON);
#endif                
            }

            void FaucetGeom::setPosition(const osg::Vec2& pos) {
                Vec3 newPos(pos.x(), pos.y(), 50);
                _geom->setPosition(newPos);
                if (!_dragger->isDirty()) {
                    _dragger->setPosition(newPos);
                }
            }
            
            
            void FaucetGeom::setEnabled(bool enabled) {
                // TODO: convert to switch usage (see FaucetGeom)
                setNodeMask(enabled ? 0xffffffff : 0);
                // Hack... really need to change to a switch node arragement.
                if (_dragger) {
                    _dragger->setNodeMask(enabled ? 0xffffffff : 0);
                }
            }
            
            void FaucetGeom::drip()  {
                // Put drip animation trigger here.
                // qDebug() << "drip" << __FILE__ << __LINE__;
            }
        };
    }
}
