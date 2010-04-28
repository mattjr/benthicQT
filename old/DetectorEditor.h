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

#ifndef __DETECTOREDITOR_H
#define __DETECTOREDITOR_H

#include <QtGui/QWidget>
#include "SamplerSet.h"

/** Forward declaration of UI implementation class. */
/** @cond */
namespace Ui {
    class DetectorEditorForm;
}
/** @endcond */

namespace ews {
    namespace app {
        /**
         * Contains classes responsible for displaying forms and gathering input from the user.
         */
        namespace widget {
            
            using ews::app::model::SamplerSet;
            using ews::app::model::PointSampler;
                
            /**
             * Form controller for editing samplers.
             */
            class DetectorEditor :  public QWidget  {
                Q_OBJECT
                
            public:
                explicit DetectorEditor(QWidget *parent = 0);
                virtual ~DetectorEditor();

                void setDataModel(SamplerSet* data);
                
                /** Remove all detectors (e.g., to start over). */
                void removeAllDetectors();

            public slots:
                /** Slot to create a new detector and add it to the list. */
                void addDetector();
                /** Slot to remove the selected detector. */
                void removeDetector();
                /** Slot to update the enabled state of the controls. */
                void updateEnabled();
                /** Update UI after the table seletion changes. */
                void updateOnSelection();
                
            private:
                void select(PointSampler* detector);
                PointSampler* selectedDetector() const;
                void syncUI();

                Ui::DetectorEditorForm* _ui;
                SamplerSet* _dataModel;
            };
        }
    }
}

#endif // __BARRIEREDITOR_H
