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

#ifndef EWSMAINWINDOW_H
#define EWSMAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtCore/QMap>
#include <osg/Node>
#include <osg/ref_ptr>
#include "BQTDefine.h"
#include "SceneRoot.h"
#include "SimulationState.h"
#include <QFileDialog>
#include "UniformCallback.hpp"
/** Forward declaration of generated UI class. */
/** @cond */
namespace Ui {
    class EWSMainWindowForm;
}
/** @endcond */


namespace ews {
    namespace app {
        namespace widget {
            using ews::app::drawable::SceneRoot;
            using ews::app::model::SimulationState;

            /**
             * Class definining the main screen, including scene graph and all widgets.
             */
            class EWSMainWindow :  public QMainWindow  {
                Q_OBJECT
                
            public:
                explicit EWSMainWindow(SimulationState* state, QWidget *parent = 0);
                virtual ~EWSMainWindow();
                
                /** Overidden to trap polish request for final initialization. */
                virtual bool event(QEvent* event);
                
                /** Get the number of milliseconds the rendering timer is set to 
                 * trigger.
                 */
                Uint getInterFrameDelay() const;

            public slots:
                /** Open file */
                void openModel();
                /** Start the simulation. */
                void start();
                /** Stop/pause the simulation. */
                void stop();
                /** Reset simulation state to initial conditions. */
                void reset();
                /** Show the preferences dialog. */
                void preferences();
                /** Show the about window. */
                void about();
                /** Request platform to open url to project website. */
                void projectWebsite();
                /** Set the inter-frame delay in milli-seconds. */
                void setInterFrameDelay(Uint delay);
                /** Show the preferences dialog. */
                void resetView();
            private slots:
                /** Do any final connecting of components after rest of application is connected.*/
                void init();
                /** Update the enabled state of menu items.*/
                void updateMenusEnabledState();

            private:
                Q_DISABLE_COPY(EWSMainWindow)
                Ui::EWSMainWindowForm* _ui;
                SimulationState* _state;
                QFileDialog *qf;
                ShaderSwitchEventHandler *sseh;
                osg::ref_ptr<SceneRoot> _sceneRoot;
            };
        }
    }
}

#endif // EWSMAINWINDOW_H
