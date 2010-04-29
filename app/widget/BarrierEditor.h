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

#ifndef __BARRIEREDITOR_H
#define __BARRIEREDITOR_H

#include <QtGui/QWidget>
#include <QDataWidgetMapper>
#include "BarrierSet.h"
#include <osg/Vec3>
#include "MeshFile.h"
/** Forward declaration of UI implementation class. */
/** @cond */
namespace Ui {
    class BarrierEditorForm;
}
/** @endcond */

namespace ews {
    namespace app {
        /**
         * Contains classes responsible for displaying forms and gathering input from the user.
         */
        namespace widget {
            
            using ews::app::model::BarrierSet;
            using ews::app::model::Barrier;
                using ews::app::model::MeshFile;
            /**
             * Form controller for editing barriers.
             */
            class BarrierEditor :  public QWidget  {
                Q_OBJECT
                
            public:
                explicit BarrierEditor(QWidget *parent = 0);
                ~BarrierEditor();
                
                /** Connect the data model instance with the editor. */
                void setDataModel(MeshFile* barriers);
                
                /** Remove all Barrier objects (e.g., to start over). */
                void removeAllBarriers();

            public slots:
                /** Slot to create a new barrier and add it to the end of the list.*/
                void addBarrier();
                /** Slot to remove the selected barrier. */
                void removeBarrier();
                /** Slot to update the enabled state of the controls. */
                void updateEnabled();
                /** Update UI after the table seletion changes. */
                void updateOnSelection();
                /** Update the data model to reflect the selected number of slits.*/
                void updatePos(osg::Vec3);
                /** Update the data model to reflect the give slit width.*/
                void updateSlitWidth(int);
                /** Update the data model to reflect the give slit separation.*/
                void updateSlitSeparation(int);
             signals:
               /**
                 * General signal for case when number of items in the
                 * set has changed. Parameter is the new number of items.
                 */
                void posChanged(osg::Vec3);

            private:
                Barrier::NumSlits numSlitsSelected() const;
                void select(Barrier* barrier);
                Barrier* selectedBarrier() const;
                void syncUI();
                QString posString;

                Ui::BarrierEditorForm* _ui;
                MeshFile* _dataModel;
            };
        }
    }
}

#endif // __BARRIEREDITOR_H
