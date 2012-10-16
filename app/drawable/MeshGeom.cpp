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
#include "LibVT_Internal.h"
#include "LibVT.h"
#include "LibVT_Config.h"
#include <osg/Drawable>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/CullFace>
#include <osg/Camera>
#include <osgDB/ReadFile>
#include "MyShaderGen.h"


#include <osg/Geode>
#include <osg/TextureRectangle>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/StateSetManipulator>

#include <osgViewer/ViewerEventHandlers>

#include <osgManipulator/Translate2DDragger>
#include "PositionHandler.h"
#include "MeshGeom.h"
#include "PotentialUpdater.h"
#include "BQTDebug.h"
#include "BarrierSet.h"
#include "SimulationState.h"
#include "ProgressBar.h"
#include "FindNode.h"


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
                
//qDebug() << " In Mesh Geom const";
         //       QObject::connect(_dataModel, SIGNAL(enableMinimap (bool)), this, SLOT(setEnabledMinimap()));

                _switch->setNewChildDefaultValue(true);
                addChild(_switch.get());
                _switch->addChild(_meshGeom.get());

                addEventCallback(new PositionHandler(&_dataModel,_dataModel.getLatOrigin(),_dataModel.getLongOrigin()));
              updateGeom();
              addEventCallback(new PickHandler(&_dataModel,_meshGeom.get()));

              osg::Vec4 color =  _dataModel.getRenderer()->getCamera()->getClearColor();
              color.r() *= 0.5f;
              color.g() *= 0.5f;
              color.b() *= 0.5f;
              _dataModel._mapSwitch=new Switch;
              _switch->addChild(_dataModel.colorbar_hud);
              _switch->addChild(_dataModel.scalebar_hud);

             _dataModel._mapSwitch->addChild(createOrthoView(_meshGeom.get(),color,_dataModel.getRenderer()->getWWManip(),
                                                  _dataModel.getRenderer()->width(),_dataModel.getRenderer()->height()));
             _dataModel._mapSwitch->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF );
              _switch->addChild(_dataModel._mapSwitch);
//                qDebug() << "In contstrt mesh gerom";
                // Callback to detect when we've been moved
                // and update the databmodel.
         //       setUpdateCallback(new PotentialUpdater);
                
                respondToSignals(true);
            }
            
            
            MeshGeom::~MeshGeom() {
            }
            
            void MeshGeom::respondToSignals(bool respond) {
                if(respond) {
               //    connect(&_dataModel, SIGNAL(dataChanged()), SLOT(updateGeom()));//, Qt::UniqueConnection);
                }
                else {
                    _dataModel.disconnect(this);
                }
            }
            


        void MeshGeom::changeOverlay(int){

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
                //qDebug() << " In Mesh Geom update";

                _meshGeom->removeChildren(0, _meshGeom->getNumChildren());
                QStringList list=_dataModel.getFileNames();
                qDebug() << "Number of files  "<<_dataModel.getFileNames().size();
                QStringList::Iterator it = list.begin();
                while( it != list.end() ) {
                   //_dataModel.getPBarD()->reset();

                    qDebug() << "Loading " << *it;
                string filename = it->toStdString();
                //osgDB::Registry::instance()->setReadFileCallback(new MyReadCallback(*sseh));
                ShaderGenReadFileCallback *readFileCallback = new ShaderGenReadFileCallback;
                   // All read nodes will inherit root state set.

                osg::ref_ptr<osgDB::ReaderWriter> rw = osgDB::Registry::instance()->getReaderWriterForExtension(osgDB::getLowerCaseFileExtension(filename));
                std::auto_ptr<progbuf> pb(new progbuf(osgDB::findDataFile(filename),_dataModel.getPBarD()));
                if (!rw || !pb->is_open())
                {
                    string errmsg= "Error: could not open file `" + filename +"'" +" Might be plugin not found\n";
                    QString errStr=errmsg.c_str();
                    std::cerr << errmsg;
                    errorD.showMessage(errStr);
                    errorD.exec();
                    ++it;
                    continue;
                }
                _dataModel.getPBarD()->setLabelText("Loading Mesh: "+*it);
                 _dataModel.getPBarD()->setRange(0,100);
                _dataModel.getPBarD()->show();
                osg::ref_ptr<osgDB::ReaderWriter::Options> local_opt = new osgDB::ReaderWriter::Options;
                local_opt->setDatabasePath(osgDB::getFilePath(filename));
                std::istream mis(pb.get());
                osg::MatrixTransform *transRev=new osg::MatrixTransform;
                osgDB::ReaderWriter::ReadResult rr = rw->readNode(mis,local_opt);
                if (rr.validNode()) {
                    osg::ref_ptr<osg::Node> node = rr.getNode();
                    if(!node.valid()){
                        string errmsg= "Invalid Node: could not open file `" + filename +"'";
                        QString errStr=errmsg.c_str();
                        std::cerr << errmsg;
                        errorD.showMessage(errStr);
                        errorD.exec();
                    }
                    osg::CoordinateSystemNode *pCoordSystem =findTopMostNodeOfType<osg::CoordinateSystemNode>(node.get());


                    osg::StateSet *ss=_meshGeom->getOrCreateStateSet();

                    if(ss){
                        std::vector<osg::Uniform *> &shared_uniforms=_dataModel.getShaderOutUniform();
                        for(int i=0; i < shared_uniforms.size(); i++)
                            ss->addUniform(shared_uniforms[i]);

                        ss->setTextureAttribute(TEXUNIT_ATTRIB,_dataModel.getSharedTex());
                    }
                    readFileCallback->setRootStateSet(ss);
                   // osgDB::Registry::instance()->setReadFileCallback(readFileCallback);

                    if( pCoordSystem ){
                        printf("New Node type with CSN\n");
                        osg::MatrixTransform *trans =findTopMostNodeOfType<osg::MatrixTransform>(node.get());
			if(trans){
			  transRev->setMatrix(osg::Matrix::inverse(trans->getMatrix()));
			}

                        transRev->addChild(node.get());
                        std::string tex_name=osgDB::getFilePath(filename)+"/vtex";
                        osg::Node *vt_node=_dataModel.createVTStuff(transRev,tex_name);
                        if(vt_node)
                        _meshGeom->addChild(vt_node);
                        else
                            _meshGeom->addChild(transRev);
                    }else{
                    _meshGeom->addChild(node.get());

                }
                    setEnabled(true);
                   //_dataModel.setStateSet(ss);
                }else{
                    if (rr.error()) {
                        string errmsg= "Error: could not open file `" + filename +"'" +rr.message();
                        QString errStr=errmsg.c_str();
                        std::cerr << errmsg;
                        errorD.showMessage(errStr);
                        errorD.exec();
                    }
                }
                ++it;
            }



            }




        }
    }
}
