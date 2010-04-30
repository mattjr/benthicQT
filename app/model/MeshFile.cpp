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
#include "BQTDebug.h"

namespace ews {
    namespace app {
        /**
         * Contains classes responsible for business logic associated with drawable objects.
         */
        namespace model {

            
            MeshFile::MeshFile() 
//            : QObject(parent)
            {
           //    QObject::connect(this, SIGNAL(dataChanged()), this, SLOT(generatePotential()));
                progress = new QProgressDialog();
                progress->setWindowModality(Qt::WindowModal);
                progress->setCancelButtonText(0);

            }
            
           
              /** Create a new barrier and add it to the list. */
                void MeshFile::loadMesh() {
                qDebug() <<"MeshFile::loadMesh()";
                emit loadNewMeshes(this);


            }
            MeshFile::~MeshFile() {
            }

            void MeshFile::updatePos(osg::Vec3 v)
            {
               bbox_map_info info;
               if(find_closet_img_idx(_tree,v,info)){
                   QString s;
                   s=(info.leftname).c_str();
                   emit imgLabelChanged(s);
                   // qDebug()<< info.leftname.c_str();
                }else{
                    QString s;
                    s=string("N/A").c_str();
                    emit imgLabelChanged(s);
                    //qDebug()<<"No img at " << v[0] << " " << v[1] << " "<<v[2];
                }

                emit posChanged(v);
            }
            

        }
    }
}
