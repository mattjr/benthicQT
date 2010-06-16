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

#ifndef __MESHFILE_H
#define __MESHFILE_H


#include <QtCore>
#include <QObject>
#include <osg/Vec3>

#include <osg/Vec2>
#include <osg/ref_ptr>
using osg::ref_ptr;
#include "BQTDefine.h"
#include "MathUtils.h"
#include <osgDB/FileNameUtils>
#include "Bboxes.hpp"
#include <osg/Uniform>
#include <QProgressDialog>
namespace ews {
    namespace app {
        namespace model {
            using osg::Vec2;

                        
            
            /**
             * Contains the business logic for barrier objects to be drawn on the screen, as well as
             * a reference to the SlitPotential necessary for physics calculations.
             */
            class MeshFile :  public QObject  {
                Q_OBJECT
                Q_PROPERTY(QStringList filenames READ getFileNames WRITE setFileNames)
            public slots:
                        void openCurrentImage();
            public:
                void loadMesh();
                double getLatOrigin(){return latOrigin;}
                double getLongOrigin(){return longOrigin;}

                void updateImage(osg::Vec3 v);

                /**
                 * Default constructor.
                 * @param parent BarrierSet this Barrier belongs to.
                 */
                explicit MeshFile();
                
                virtual ~MeshFile();

                


                /**
                 * Gets enabled state of barrier. 
                 * @return true if active, false if inactive and invisible.
                 */
               /* bool isEnabled() const {
                    return _enabled;
                }*/
                void updateBoxes(){
                    QStringList list=getFileNames();
                    QStringList::Iterator it = list.begin();
                    while( it != list.end() ) {
                        string path=osgDB::getFilePath(it->toStdString());
                        _tree=loadBBox(string(path+"/campath.txt").c_str());
                        if(_tree)
                            qDebug() << "Sucessfully loaded Tree";
                        it++;
                    }
                }
                void updateShaders(){
                    QStringList list=getFileNames();
                    QStringList::Iterator it = list.begin();
                    while( it != list.end() ) {
                        string path=osgDB::getFilePath(it->toStdString());
                        FILE *fp=fopen(string(path+"/shaderout.txt").c_str(),"r");
                        if(fp){
                            fscanf(fp,"%d\n",&num_shader_out);
                            qDebug() << "Sucessfully loaded shaderout";
                            fclose(fp);
                        }
                        it++;
                    }
                }
                void updateOrigin(){
                    QStringList list=getFileNames();
                    QStringList::Iterator it = list.begin();
                    while( it != list.end() ) {
                        string path=osgDB::getFilePath(it->toStdString());
                        FILE *fp=fopen(string(path+"/origin.txt").c_str(),"r");
                        if(fp){
                            fscanf(fp,"%lf %lf\n",&latOrigin,&longOrigin);
                            qDebug() << "Sucessfully loaded shaderout";
                            fclose(fp);
                        }else{
                            latOrigin=0;
                            longOrigin=0;

                        }
                        it++;
                    }
                }
                void setFileNames(QStringList files) {
                    filenames = files;
//                    qDebug() << "SizeFM " << files.size();
                    updateBoxes();
                    updateShaders();
                    updateOrigin();
                    //emit dataChanged();
                }
            void setShaderOut(int index);
                QStringList getFileNames(void) {
                    return filenames ;

                }
                std::vector<string> getShaderNames(void) {
                    return shader_names ;

                }
                int getNumShaderOut(void) {
                    return num_shader_out ;

                }

                osg::Uniform * getShaderOutUniform(){return shared_shader_out;}
                

                void updatePos(osg::Vec3 v);

                QProgressDialog *getPBarD(void){
                        return progress;
                    }
            signals:
               // void dataChanged();
                //void potentialChanged();
                void loadNewMeshes(MeshFile *);

                /**
                 * General signal for case when number of items in the
                 * set has changed. Parameter is the new number of items.
                 */
                void posChanged(osg::Vec3);
                void imgLabelChanged(QString);


            private slots:
              //  void generatePotential();
                
            private:
                Q_DISABLE_COPY(MeshFile)
                QString curr_img;

                //bool _enabled;
                QStringList filenames;
                RTree *_tree;
                QProgressDialog *progress;
                osg::Uniform* shared_shader_out;
                int num_shader_out;
                double latOrigin, longOrigin;
                 std::vector<string>  shader_names;
            };
        }
    }
}


#endif // __BARRIER_H

