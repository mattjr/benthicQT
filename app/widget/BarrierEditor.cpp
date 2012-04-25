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

#include "Barrier.h"
#include "BarrierEditor.h"
#include "BarrierTableModel.h"
#include <QDesktopServices>

#include <QTableView>
#include <QModelIndex>
#include <QItemDelegate>
#include <QItemSelectionModel>
#include "ui_BarrierEditor.h"
#include "BQTDefine.h"
namespace ews {
    namespace app {
        namespace widget {
            using ews::app::model::BarrierSet;
            using ews::app::model::Barrier;
            
            BarrierEditor::BarrierEditor(QWidget* parent) 
            : QWidget(parent), _ui(new Ui::BarrierEditorForm), _dataModel(NULL) {
                _ui->setupUi(this);


                updateEnabled();
            }
            
            BarrierEditor::~BarrierEditor() {
                _dataModel = NULL; // We don't own the data model
                delete _ui;
            }
            
            /** Update  controls with current widget state. */
            void BarrierEditor::syncUI() {
             /*   Barrier* b = selectedBarrier();
                
                if(b) {
                    blockSignals(true);
                    
                    _ui->zeroSlits->setChecked(b->getNumSlits() == Barrier::ZERO_SLITS);
                    _ui->oneSlit->setChecked(b->getNumSlits() == Barrier::ONE_SLIT);
                    _ui->twoSlits->setChecked(b->getNumSlits() == Barrier::TWO_SLITS);
                    
                    
                    _ui->slitWidth->setValue(b->getSlitWidth());
                    _ui->slitSeparation->setValue(b->getSlitSeparation());

                    blockSignals(false);
                }*/
            }
            void BarrierEditor::displayMeasure(osg::Vec3 dist,osg::Vec3 diff){
                char tmp[1024];
                sprintf(tmp,"Dist: %.2f X: %.2f Y: %.2f Z: %.2f\n",dist[0],diff[0],diff[1],diff[2]);
                QString statusStr=tmp;
                _ui->status_line->setText(statusStr);

            }

            /** Apply datamodel to editor. */
            void BarrierEditor::setDataModel(MeshFile* mf) {
                _dataModel = mf;

                  QObject::connect(_dataModel, SIGNAL(posChanged(osg::Vec4)), this, SLOT(updatePos(osg::Vec4)));
                 QObject::connect(_dataModel, SIGNAL(imgLabelChanged (QString)), this, SLOT(updateImgLabel(QString)));
        //         QObject::connect(_dataModel, SIGNAL(imgLabelChanged (QString)), this, SLOT(updateImgLabel(QString)));


                syncUI();
                updateEnabled();
            }
            

 


            
            /** Update the enabled state of all the widgets. */
            void BarrierEditor::updateEnabled() {

            }
            void BarrierEditor::updateDataUsedWidget(int index){
                datausedCombo->blockSignals(true);
                colormapCombo->blockSignals(true);
                datausedCombo->clear();
                colormapCombo->clear();


                if(_dataModel->getColorMapNames()){
                    for(int i=0; i< _dataModel->getColorMapNames()->size(); i++)
                        colormapCombo->addItem((*_dataModel->getColorMapNames())[i]);
                }
                for(int i=0; i< _dataModel->getDataUsedNames().size(); i++)
                    datausedCombo->addItem(_dataModel->getDataUsedNames()[i].c_str());
                /*   else{
                       char tmp[255];
                       sprintf(tmp,"Aux %d",i);
                       QString qtmp=tmp;
                       overlay->addItem(qtmp);
                       //qDebug() << "Adding " <<qtmp;
                   }

               }*/
                datausedCombo->setCurrentIndex(index);
                datausedCombo->blockSignals(false);
                colormapCombo->blockSignals(false);
                colormapCombo->setCurrentIndex(0);
                changeColorMap(0);

            }
            void BarrierEditor::updateOverlayWidget(){
                overlay->blockSignals(true);

                overlay->clear();


                for(int i=0; i< _dataModel->getShaderNames().size(); i++)
                    overlay->addItem(_dataModel->getShaderNames()[i].c_str());

                overlay->blockSignals(false);

            }
            void BarrierEditor::updatePos(osg::Vec4 pos) {
                QObject* sender = QObject::sender();
                if(sender) {
                    char tmp[1024];
                    int labelId=(int)round(pos[3]);
                    double label=0.0;
                    if(labelId > 0 &&  labelId  <_dataModel->current_attributes.size()){
                       label= _dataModel->current_attributes[labelId];
                    sprintf(tmp,"Lat: %3.8f Long: %3.8f Z: %3.2f Label: %.0f",pos[0],pos[1],pos[2],label);
                }else
                    sprintf(tmp,"Lat: %3.8f Long: %3.8f Z: %3.2f",pos[0],pos[1],pos[2]);

                    posString=tmp;
                    _ui->label_2->setText(posString);

                }
            }
                void BarrierEditor::updateImgLabel(QString str) {
                QObject* sender = QObject::sender();
                if(sender) {
                    _ui->lineEdit->setText(str);

                }
            }


            void BarrierEditor::changeOverlay(int index)
            {
             // qDebug() << "changed to " << index;
                if(_dataModel)
                    _dataModel->setShaderOut(index);
                if(index ==0){
                    opacitySlider->setDisabled(false);
                }else{
                    opacitySlider->setDisabled(true);;
                }
            }

            void BarrierEditor::changeOpacity(int val)
            {
             //   qDebug() << "changed to " << index;
                if(_dataModel)
                    _dataModel->setOpacity(val);
            }

            void BarrierEditor::changeDataUsed(int index)
            {
             //   qDebug() << "changed to " << index;
                if(_dataModel)
                    _dataModel->setDataUsed(index);
            }
            void BarrierEditor::changeColorMap(int index)
            {
             //   qDebug() << "changed to " << index;
                if(_dataModel)
                    _dataModel->setColorMap(index);
            }
        }
    }
}

