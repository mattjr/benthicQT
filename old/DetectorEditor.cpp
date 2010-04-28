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

#include "PointSampler.h"
#include "DetectorEditor.h"
#include "DetectorTableModel.h"

#include <QTableView>
#include <QModelIndex>
#include <QItemDelegate>
#include <QItemSelectionModel>
#include "ui_DetectorEditor.h"
#include "EWSDefine.h"
#include "SamplerSet.h"

namespace ews {
    namespace app {
        namespace widget {
            
            using ews::app::model::SimulationState;
            using ews::app::model::PointSampler;
            
            DetectorEditor::DetectorEditor(QWidget* parent) 
            : QWidget(parent), _ui(new Ui::DetectorEditorForm), _dataModel(NULL) {
                _ui->setupUi(this);   
                updateEnabled();
            }
            
            DetectorEditor::~DetectorEditor() {
                // We don't own the dataModel.
                _dataModel = NULL;
                delete _ui;
            }
            
            /** Update  controls with current widget state. */
            void DetectorEditor::syncUI() {
                PointSampler* p = selectedDetector();
                
                if(p) {
                    blockSignals(true);
                    // Highlight sampler.
                    blockSignals(false);
                }
            }

            /** Apply datamodel to editor. */
            void DetectorEditor::setDataModel(SamplerSet* samplers) {
                _dataModel = samplers;
                
                QAbstractItemModel* model = _ui->samplerTable->model();
                if(model) {
                    _ui->samplerTable->setModel(NULL);
                    QItemSelectionModel* sm = _ui->samplerTable->selectionModel();
                    if(sm) {
                        sm->disconnect(this);
                        delete sm;
                    }
                    model->disconnect(this);
                    delete model;
                }
                
                if(_dataModel) {
                    QAbstractItemModel* newModel = new DetectorTableModel(_dataModel);
                    
                    _ui->samplerTable->setModel(newModel);
                    
                    _ui->samplerTable->horizontalHeader()->setStretchLastSection(false);
                    _ui->samplerTable->horizontalHeader()->setResizeMode(DetectorTableModel::NAME, QHeaderView::Stretch);
                    
                    _ui->samplerTable->setColumnWidth(DetectorTableModel::ENABLED, 25);
                    _ui->samplerTable->setColumnWidth(DetectorTableModel::COLOR, 25);
                    
                    // Register for selection changes so we can update the widgets. */
                    connect(_ui->samplerTable->selectionModel(), 
                            SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
                            this, SLOT(updateOnSelection()));                    
                    connect(newModel, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
                            this, SLOT(updateEnabled()));                    
                }
                
                syncUI();
                updateEnabled();
            }
            
            void DetectorEditor::select(PointSampler* sampler) {
                int i = sampler != NULL ? _dataModel->indexOf(sampler) : -1;
                if (i >= 0) {
                    QModelIndex idx = _ui->samplerTable->model()->index(i, 0);
                    _ui->samplerTable->setCurrentIndex(idx);
                }
                else {
                    QModelIndex idx = _ui->samplerTable->model()->index(0, 0);
                    _ui->samplerTable->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Clear);
                }

            }
            
            PointSampler* DetectorEditor::selectedDetector() const {
                if(!_dataModel) return NULL;
                QModelIndex idx = _ui->samplerTable->currentIndex();
                return idx.row() >= 0 ? _dataModel->samplerAt(idx.row()) : NULL;
            }
            
            void DetectorEditor::updateOnSelection() {
                syncUI();
                updateEnabled();
            }

            void DetectorEditor::addDetector() {
                PointSampler* b = _dataModel->createSampler();
                select(b);
                _ui->samplerTable->resizeRowsToContents();
            }

            void DetectorEditor::removeAllDetectors() {
                _dataModel->removeAllSamplers();
                select(NULL);
                updateEnabled();
                _ui->samplerTable->resizeRowsToContents();
            }

            void DetectorEditor::removeDetector() {
                PointSampler* s = selectedDetector();
                _dataModel->removeSampler(s);
                if (_dataModel->size() > 0) {
                    QModelIndex idx = _ui->samplerTable->currentIndex();
                    int row = (idx.row() < 0) ? 0 :
                                 (idx.row() >= _dataModel->size()) ? _dataModel->size() - 1 : idx.row();
                    select(_dataModel->samplerAt(row));
                }
                else {
                    select(NULL);
                }
                updateEnabled();
            }

            
            void DetectorEditor::updateEnabled() {
                bool canEdit = false;
                if(_ui->samplerTable->selectionModel()) {
                    canEdit = _ui->samplerTable->selectionModel()->hasSelection();
                }
                
                if (canEdit) {
                    PointSampler* s = selectedDetector();
                    canEdit = s != NULL && s->isEnabled();
                }
                _ui->remove->setEnabled(canEdit);
                
                bool canAdd = _dataModel != NULL && !_dataModel->isFull();
                _ui->add->setEnabled(canAdd);
            }

        }
    }
}
