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
#include <osgDB/ReadFile>
#include <osgManipulator/Translate2DDragger>
#include "PositionHandler.h"
#include "MeshGeom.h"
#include "PotentialUpdater.h"
#include "BQTDebug.h"
#include "BarrierSet.h"
#include "SimulationState.h"
namespace ews {
    namespace app {
        namespace drawable {
            using namespace osg;
            using namespace osgManipulator;
            using namespace app::model;
            

            
            /** Primary constructor. */
            MeshGeom::MeshGeom(MeshFile& dataModel)
            : DrawableQtAdapter(), _dataModel(dataModel), _switch(new Switch),
            _meshGeom(new PositionAttitudeTransform)
       {
                

                
                _switch->setNewChildDefaultValue(true);
                addChild(_switch.get());
                _switch->addChild(_meshGeom.get());
                addEventCallback(new PositionHandler(&_dataModel));

                
                updateGeom();
                
                // Callback to detect when we've been moved
                // and update the databmodel.
         //       setUpdateCallback(new PotentialUpdater);
                
                respondToSignals(true);
            }
            
            
            MeshGeom::~MeshGeom() {
            }
            
            void MeshGeom::respondToSignals(bool respond) {
                if(respond) {
                    connect(&_dataModel, SIGNAL(dataChanged()), SLOT(updateGeom()));//, Qt::UniqueConnection);
                }
                else {
                    _dataModel.disconnect(this);
                }
            }
            




            void MeshGeom::setEnabled(bool enabled) {
                if(enabled) {
                    _switch->setAllChildrenOn();
                }
                else {
                    _switch->setAllChildrenOff();
                }
            }
            
            void MeshGeom::updateGeom() {
                _meshGeom->removeChildren(0, _meshGeom->getNumChildren());
                QStringList list=_dataModel.getFileNames();
                QStringList::Iterator it = list.begin();
                while( it != list.end() ) {
                    qDebug() << "Loading " << *it;
                osg::ref_ptr<osg::Node> Node = osgDB::readNodeFile( it->toStdString());
                _meshGeom->addChild(Node.get());
                    ++it;
                }
                setEnabled(_dataModel.isEnabled());
            }


        }
    }
}
