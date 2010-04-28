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

#include "SceneRoot.h"
#include <osg/ShapeDrawable>

#include <osg/Light>
#include <osg/LightSource>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgManipulator/Dragger>
#include "DrawableFactory.h"
#include "PickHandler.h"

namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            
            SceneRoot::SceneRoot(QObject* parent) 
            : QObject(parent), osg::MatrixTransform(), _drawables() {
                // Set up some basic lighting.
                
                /*osg::StateSet* state = getOrCreateStateSet();
                state->setMode( GL_LIGHTING, osg::StateAttribute::ON ); 
                
                state->setMode( GL_LIGHT0, osg::StateAttribute::ON );
                
                ref_ptr<Light> light = new Light; 
                light->setLightNum(1);
                light->setPosition(Vec4(10.0f, 10.0f, 100.0f, 1.0f)); 
                light->setAmbient(Vec4(0.1f, 0.1f, 0.1f, 1.0f)); 
                light->setDiffuse(Vec4(0.5f, 0.5f, 0.5f, 1.0f));
                light->setSpecular(Vec4(0.8f, 0.8f, 0.8f, 1.0f)); 
                
                ref_ptr<LightSource> lSource = new LightSource;
                lSource->setLight(light.get());
                lSource->setReferenceFrame(LightSource::ABSOLUTE_RF );
                addChild(lSource.get());*/
            //    osg::ref_ptr<osg::Node> Cow = osgDB::readNodeFile("/home/mattjr/11_elephant_rock/final.ive");
             //  addChild(Cow.get());
                osg::ref_ptr<osg::Geode> geode = new osg::Geode();

                osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0,0,0), 1.5f, 1.5f, 4.0f);
                osg::ref_ptr<osg::ShapeDrawable> drawable = new osg::ShapeDrawable(box.get());
                geode->addDrawable(drawable.get());
                addChild(geode);
                addEventCallback(new PickHandler);
            }
            
            
            SceneRoot::~SceneRoot() {
            }

            
            void SceneRoot::addDrawableFor(QObject& data) {
                ref_ptr<DrawableQtAdapter> drawable = DrawableFactory::instance().createDrawableFor(data);
                if(drawable) {
                   addChild(drawable);
                    _drawables.insert(&data, drawable);
                }
            }
            
            void SceneRoot::removeDrawableFor(QObject& data) {
                ref_ptr<Node> geom = _drawables.take(&data);
                if(geom) {
                    qDebug() << "removing a drawable node" << geom <<"of type" << typeid(*geom).name();
                    removeChild(geom);
                }
            }

        }
    }
}
