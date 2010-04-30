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
#include "ProgressBar.h"

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
                qDebug() << "Number of files  "<<_dataModel.getFileNames().size();
                QStringList::Iterator it = list.begin();
                while( it != list.end() ) {
                   //_dataModel.getPBarD()->reset();

  //                  qDebug() << "Loading " << *it;
                string filename = it->toStdString();
                osg::ref_ptr<osgDB::ReaderWriter> rw = osgDB::Registry::instance()->getReaderWriterForExtension(osgDB::getLowerCaseFileExtension(filename));
                std::auto_ptr<progbuf> pb(new progbuf(osgDB::findDataFile(filename),_dataModel.getPBarD()));
                if (!rw || !pb->is_open())
                {
                    string errmsg= "Error: could not open file `" + filename +"'" +" Might be plugin not found\n";
                    QString errStr=errmsg.c_str();
                    std::cerr << errmsg;
                    errorD.showMessage(errStr);
                    ++it;
                    continue;
                }
                _dataModel.getPBarD()->setLabelText("Loading Mesh: "+*it);
                 _dataModel.getPBarD()->setRange(0,100);
                _dataModel.getPBarD()->show();

                std::istream mis(pb.get());
                osgDB::ReaderWriter::ReadResult rr = rw->readNode(mis);
                osg::ref_ptr<osg::Node> node = rr.getNode();
                _meshGeom->addChild(node.get());
                ++it;
            }


                setEnabled(_dataModel.isEnabled());
                _dataModel.getPBarD()->close();
                qApp->processEvents();

            }


        }
    }
}
