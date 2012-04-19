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
#include <osgSim/ScalarBar>
#include "Bboxes.hpp"
#include <osg/Uniform>
#include <QProgressDialog>
#include "QOSGWidget.h"
namespace ews {
    namespace app {
        namespace model {
            using osg::Vec2;
            using namespace ews::app::widget;
                        
            
            /**
             * Contains the business logic for barrier objects to be drawn on the screen, as well as
             * a reference to the SlitPotential necessary for physics calculations.
             */
            class MeshFile :  public QObject  {
                Q_OBJECT
                Q_PROPERTY(QStringList filenames READ getFileNames WRITE setFileNames)
            public slots:
                        void openCurrentImage();
                void switchMinimap(bool enabled);
                void copyCurrentImageClipboard();

            public:
                void setStateSet(osg::StateSet *state);

                void loadMesh();
                double getLatOrigin(){return latOrigin;}
                double getLongOrigin(){return longOrigin;}
                void updateGlobal(osg::Vec3 v);
                QOSGWidget * getRenderer(){return _renderer;}
                void setRenderer(QOSGWidget *r){_renderer=r;} 
                void updateImage(osg::Vec3 v);
                osg::ref_ptr<osg::Switch> _mapSwitch;
                osg::ref_ptr<osg::Projection > colorbar_hud;
                osg::ref_ptr<osgSim::ScalarBar > colorbar;

                void createScalarBar_HUD(void);

                /**
                 * Default constructor.
                 * @param parent BarrierSet this Barrier belongs to.
                 */
                explicit MeshFile(QOSGWidget *render);
                
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
                        if(path.size() ==0)
                            path=".";
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
                        if(path.size() ==0)
                            path=".";
                        FILE *fp=fopen(string(path+"/zrange.txt").c_str(),"r");
                        if(fp){
                            fscanf(fp,"%f %f\n",&zrange[0],&zrange[1]);
                            qDebug() << "Sucessfully loaded zrange";
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
                        if(path.size() ==0)
                            path=".";
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
                    createScalarBar_HUD();

                    updateBoxes();
                    updateShaders();
                    updateOrigin();
                    setDataUsed(0);
                    setColorMap(0);
                    updateSharedAttribTex();
                    //emit dataChanged();
                }
            void setShaderOut(int index);
            void setColorMap(int index);
            void setDataUsed(int index);
            void setDataRange(osg::Vec2 range);
            void updateSharedAttribTex();

            enum{
                HEIGHT_DATA
            };
            enum{
                UNI_SHADER_OUT,
                UNI_COLORMAP,
                UNI_DATAUSED,
                UNI_VAL_RANGE,
                UNI_TEXSCALE,
                NUM_UNI_ENUM
            }uniform_list;

                QStringList getFileNames(void) {
                    return filenames ;

                }
                std::vector<string> getShaderNames(void) {
                    return shader_names ;

                }

                std::vector<string> getColorMapNames(void) {
                    return colormap_names ;

                }
                std::vector<string> getDataUsedNames(void) {
                    return dataused_names ;

                }
                int getNumShaderOut(void) {
                    return shader_names.size() ;

                }

                std::vector<osg::Uniform *> &getShaderOutUniform(){return shared_uniforms;}
                osg::ref_ptr<osg::Texture2D> getSharedTex(){return shared_tex;}


                void updatePos(osg::Vec3 v);

                QProgressDialog *getPBarD(void){
                        return progress;
                    }
                void measureResultsEmit(osg::Vec3 v,osg::Vec3 v2){
                    emit measureResults(v,v2);

                }

            signals:
               // void dataChanged();
                //void potentialChanged();
                void loadNewMeshes(MeshFile *);
                void enableMinimap(bool);
                /**
                 * General signal for case when number of items in the
                 * set has changed. Parameter is the new number of items.
                 */
                void posChanged(osg::Vec3);
                void imgLabelChanged(QString);
                void measureResults(osg::Vec3,osg::Vec3);


            private slots:
              //  void generatePotential();
                
            private:
                Q_DISABLE_COPY(MeshFile)
                QString curr_img;

                //bool _enabled;
                QStringList filenames;
                RTree *_tree;
                QProgressDialog *progress;
                std::vector<osg::Uniform*> shared_uniforms;
                double latOrigin, longOrigin;
                 std::vector<string>  shader_names;
                 std::vector<string>  colormap_names;
                 std::vector<string>  dataused_names;
                 osg::ref_ptr<osg::Texture2D> shared_tex;
                 osg::ref_ptr<osg::Image> dataImage;
                 QOSGWidget *_renderer;
                 osg::StateSet *_stateset;
                 osg::Vec2f zrange;
                 std::vector<double> current_attributes;
            };
        }
    }
}


#endif // __BARRIER_H

