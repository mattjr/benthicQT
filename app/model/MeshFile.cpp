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



#include "MeshFile.h"
//#include "SimulationState.h"
#include <osg/Vec2>
#include <osg/ref_ptr>
#include <QDesktopServices>
#include "BQTDebug.h"
#include "BQTMainWindow.h"

namespace ews {
    namespace app {
        /**
         * Contains classes responsible for business logic associated with drawable objects.
         */
        namespace model {

            
            MeshFile::MeshFile(QOSGWidget *renderer): _renderer(renderer)
                    //            : QObject(parent)
            {
                //    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(generatePotential()));
                progress = new QProgressDialog();
                progress->setWindowModality(Qt::WindowModal);
                progress->setCancelButtonText(0);
                shared_shader_out = new osg::Uniform("shaderOut",0);
                num_shader_out=4;
                shader_names.push_back("Texture");
                shader_names.push_back("Label");
                shader_names.push_back("Overlay");
                shader_names.push_back("Shaded");
                //_manip=

            }
            

            /** Create a new barrier and add it to the list. */
            void MeshFile::loadMesh() {
                qDebug() <<"MeshFile::loadMesh()";
                emit loadNewMeshes(this);


            }
            MeshFile::~MeshFile() {
            }

            void MeshFile::openCurrentImage()
            {
                std::string imgdir_file;
                char reldir[1024];
                string path;
                QStringList list=getFileNames();
                QStringList::Iterator it = list.begin();
                while( it != list.end() ) {
                     path=osgDB::getFilePath(it->toStdString());
                    imgdir_file=string(path+"/imgpath.txt");
                    FILE *fp=fopen(imgdir_file.c_str(),"r");
                    if(fp)
                        fscanf(fp,"%s\n",reldir);
                    else
                        printf("Can't open %s",imgdir_file.c_str());
                    it++;
                }
                QUrl url;
                QString fullpath=string(path+"/"+reldir+"/"+curr_img.toStdString()).c_str();
                qDebug() << fullpath<<"\n";
                if(curr_img.size())
                    url.setUrl(fullpath);
                QDesktopServices::openUrl(url);
            }
            void MeshFile::updateImage(osg::Vec3 v)
            {
                bbox_map_info info;
                if(find_closet_img_idx(_tree,v,info)){
                    curr_img=(info.leftname).c_str();
                    emit imgLabelChanged(curr_img);
                }else{
                    curr_img="";
                    QString s;
                    s=string("N/A").c_str();
                    emit imgLabelChanged(s);
                    //qDebug()<<"No img at " << v[0] << " " << v[1] << " "<<v[2];
                }

            }
            void MeshFile::updateGlobal(osg::Vec3 v){
                emit posChanged(v);

            }

            void MeshFile::switchMinimap(bool enabled){
                    if(enabled) {
                        _mapSwitch->setAllChildrenOn();
                    }
                    else {
                        _mapSwitch->setAllChildrenOff();
                    }

            }

            void MeshFile::setShaderOut(int index) {
                if(shared_shader_out)
                    shared_shader_out->set(index);
            }

            

        }
    }
}
