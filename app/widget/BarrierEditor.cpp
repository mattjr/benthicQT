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
                Barrier* b = selectedBarrier();
                
                if(b) {
                    blockSignals(true);
                    
                    /*_ui->zeroSlits->setChecked(b->getNumSlits() == Barrier::ZERO_SLITS);
                    _ui->oneSlit->setChecked(b->getNumSlits() == Barrier::ONE_SLIT);
                    _ui->twoSlits->setChecked(b->getNumSlits() == Barrier::TWO_SLITS);
                    
                    
                    _ui->slitWidth->setValue(b->getSlitWidth());
                    _ui->slitSeparation->setValue(b->getSlitSeparation());
                    */
                    blockSignals(false);
                }
            }

            /** Apply datamodel to editor. */
            void BarrierEditor::setDataModel(MeshFile* mf) {
                _dataModel = mf;
                  QObject::connect(_dataModel, SIGNAL(posChanged(osg::Vec3)), this, SLOT(updatePos(osg::Vec3)));
                 QObject::connect(_dataModel, SIGNAL(imgLabelChanged (QString)), this, SLOT(updateImgLabel(QString)));

            /*    QAbstractItemModel* model = _ui->barrierTable->model();
                if(model) {
                    _ui->barrierTable->setModel(NULL);
                    QItemSelectionModel* sm = _ui->barrierTable->selectionModel();
                    if(sm) {
                        sm->disconnect(this);
                        delete sm;
                    }
                    model->disconnect(this);
                    delete model;
                }
                
                if(_dataModel) {
                    QAbstractItemModel* newModel = new BarrierTableModel(_dataModel);
                    
                    _ui->barrierTable->setModel(newModel);
                    
                    
                    // Hide columns we don't want in the table.
                    for(Uint c = BarrierTableModel::NAME + 1; c < BarrierTableModel::NUM_COLS; c++) {
                        _ui->barrierTable->setColumnHidden(c, true);
                    }

                    // Register for selection changes so we can update the widgets.
                    connect(_ui->barrierTable->selectionModel(), 
                            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                            this, SLOT(updateOnSelection()));                    
                    connect(newModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
                            this, SLOT(updateEnabled()));
                }*/
                
                syncUI();
                updateEnabled();
            }
            
            void BarrierEditor::select(Barrier* barrier) {
                /*int i = barrier != NULL ? _dataModel->indexOf(barrier) : -1;
                if (i >= 0) {
                    QModelIndex idx = _ui->barrierTable->model()->index(i, 0);
                    _ui->barrierTable->setCurrentIndex(idx);
                }
                else {
                    QModelIndex idx = _ui->barrierTable->model()->index(0, 0);
                    _ui->barrierTable->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Clear);
                }*/

            }
            
            Barrier* BarrierEditor::selectedBarrier() const {
              /*  if(!_dataModel) return NULL;
                QModelIndex idx = _ui->barrierTable->currentIndex();
                return idx.row() >= 0 ? _dataModel->barrierAt(idx.row()) : NULL;
           */ }
            
            void BarrierEditor::updateOnSelection() {
                syncUI();
                updateEnabled();
            }

            void BarrierEditor::addBarrier() {
             /*   Barrier* b = _dataModel->createBarrier();
                select(b);
                _ui->barrierTable->resizeRowsToContents();
            */
            }

            void BarrierEditor::removeAllBarriers() {
            /*    _dataModel->removeAllBarriers();
                select(NULL);
                updateEnabled();
                _ui->barrierTable->resizeRowsToContents();
            */
            }

            void BarrierEditor::removeBarrier() {
             /*   Barrier* b = selectedBarrier();
                _dataModel->removeBarrier(b);
                if (_dataModel->size() > 0) {
                    QModelIndex idx = _ui->barrierTable->currentIndex();
                    int row = (idx.row() < 0) ? 0 :
                                 (idx.row() >= _dataModel->size()) ? _dataModel->size() - 1 : idx.row();
                    select(_dataModel->barrierAt(row));
                }
                else {
                    select(NULL);
                }
                updateEnabled();
            */}
            
            Barrier::NumSlits BarrierEditor::numSlitsSelected() const {
                /*if(_ui->zeroSlits->isChecked()) {
                    return Barrier::ZERO_SLITS;
                }
                else if(_ui->oneSlit->isChecked()) {
                    return Barrier::ONE_SLIT;
                }
                else if(_ui->twoSlits->isChecked()) {
                    return Barrier::TWO_SLITS;
                }
                return Barrier::ZERO_SLITS;*/
            }
            
            /** Update the enabled state of all the widgets. */
            void BarrierEditor::updateEnabled() {
                bool canEdit = false;
                if(_ui->barrierTable->selectionModel()) {
                    canEdit = _ui->barrierTable->selectionModel()->hasSelection();
                }
                
                _ui->removeBarrier->setEnabled(canEdit);

                // Tweak canEdit determination with enabled state. */
                if (canEdit) {
                    Barrier* b = selectedBarrier();
                    canEdit = b != NULL && b->isEnabled();
                }

                //bool canAdd = _dataModel != NULL && !_dataModel->isFull();
                //_ui->addBarrier->setEnabled(canAdd);

                /*_ui->slitNumSelect->setEnabled(canEdit);
                
                Barrier::NumSlits num = numSlitsSelected();
                 
                const bool canEditSlit = canEdit && num != Barrier::ZERO_SLITS;
                _ui->slitWidth->setEnabled(canEditSlit);
                const bool canEditSlitSep = canEdit && num == Barrier::TWO_SLITS;
                _ui->slitSeparation->setEnabled(canEditSlitSep);*/
            }
            
            void BarrierEditor::updatePos(osg::Vec3 pos) {
                QObject* sender = QObject::sender();
                if(sender) {
                    char tmp[1024];
                    sprintf(tmp,"X: %.2f\nY: %.2f\nZ: %.2f",pos[0],pos[1],pos[2]);
                    posString=tmp;
                    _ui->label_2->setText(posString);

                }
            }
                void BarrierEditor::updateImgLabel(QString str) {
                QObject* sender = QObject::sender();
                if(sender) {
                    _ui->textBrowser->setText(str);

                }
            }
            
            void BarrierEditor::updateSlitWidth(int width) {
                Barrier* b = selectedBarrier();
                if(b) {
                    b->setSlitWidth(static_cast<Uint>(width));
                }
            }

            void BarrierEditor::updateSlitSeparation(int sep) {
                Barrier* b = selectedBarrier();
                if(b) {
                    b->setSlitSeparation(static_cast<Uint>(sep));
                }
            }
        }
    }
}
