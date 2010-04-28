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

#include "DripSourceEditor.h"
#include "ui_DripSourceEditor.h"
#include "DripSource.h"
#include "ui_EWSMainWindow.h"
#include "EWSDebug.h"

namespace ews {
    namespace app {
        namespace widget {
            DripSourceEditor::DripSourceEditor(QWidget* parent) 
            : QWidget(parent), _ui(new Ui::DripSourceEditorForm), _dataModel(NULL) {
                
                _ui->setupUi(this);   

            }
            
            DripSourceEditor::~DripSourceEditor() {
                _dataModel = NULL; // We don't own the data model.
                delete _ui;
            }
            
            void DripSourceEditor::setDataModel(DripSource* data) {
                if (_dataModel != NULL) {
                    _dataModel->disconnect(this);
                }
                
                _dataModel = data;
                
                syncUI();
            }
            
            void DripSourceEditor::setEnabled(bool state) {
                if (_dataModel != NULL) {
                    bool prevState = _dataModel->isEnabled();
                    if (prevState != state) {
                        _dataModel->setEnabled(state);                        
                        emit enabledChanged(state);
                    }
                }
                syncUI();
            }
            
            
            void DripSourceEditor::syncUI() {
                if (_dataModel != NULL) {
                    blockSignals(true);
                    _ui->enabled->setChecked(_dataModel->isEnabled());
                    _ui->throbber->setEnabled(_dataModel->isEnabled());
                    _ui->frequency->setValue(_dataModel->getFrequency());
                    _ui->amplitude->setValue(_dataModel->getAmplitude());    
                    blockSignals(false);
                }
            }
            
            void DripSourceEditor::pulse() {
                if(_dataModel) {
                    // When the user clicks the "pulse" button it 
                    // sets the frequency to zero to basically turn off the
                    // oscillator.
                    _ui->frequency->setValue(0);
                    _dataModel->pulseDrip();
                }
            }
        }
    }
}
