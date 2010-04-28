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

#ifndef _DRIPSOURCEEDITOR_H
#define _DRIPSOURCEEDITOR_H

#include <QtGui/QWidget>
#include "DripSource.h"
#include "EWSDebug.h"

/** Forward declaration of UI implementation class. */
/** @cond */
namespace Ui {
    class DripSourceEditorForm;
}
/** @endcond */


namespace ews {
    namespace app {
        namespace widget {
            using namespace ews::util::debug;            
            using ews::app::model::DripSource;
            
            /**
             * Provides a UI for the end-user to be able to manipulate DripSource objects, as well
             * as a reference to those DripSource objects.
             */
            class DripSourceEditor :  public QWidget  {
                Q_OBJECT
                
            public:
                explicit DripSourceEditor(QWidget *parent = 0);
                virtual ~DripSourceEditor();
                
                void setDataModel(DripSource* data);
                
            public slots:
                virtual void setEnabled(bool state);
                void pulse();
                
                void setAmplitude(int val) {
                    if(_dataModel) {
                        _dataModel->setAmplitude(val);
                    }
                }
                
                void setFrequency(int val) {
                    if(_dataModel) {
                        _dataModel->setFrequency(val);
                    }
                }

                void syncUI();

            signals:
                void enabledChanged(bool);
                
            private:
                Ui::DripSourceEditorForm* _ui;
                DripSource* _dataModel;
            };
        }
    }
}

#endif // _DRIPSOURCEEDITOR_H
