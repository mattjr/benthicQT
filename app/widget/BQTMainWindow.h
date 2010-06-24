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
#include <QComboBox>
#include "SimulationState.h"
#include <QFileDialog>
#include "SavedCameraWidget.h"

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
            using ews::app::model::MeshFile;

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
                 bool eventFilter(QObject * watched, QEvent * event);
                /** Get the number of milliseconds the rendering timer is set to 
                 * trigger.
                 */
                Uint getInterFrameDelay() const;
                void setCurrentFile(const QString &fileName);
                void updateRecentFileActions();
                QString strippedName(const QString &fullFileName);
                QSize computerWindowSizeForRenderSize(QSize target);


            public slots:
                /** Open file */
                void openModel();
                /** Start the simulation. */
                void start();
                /** Stop/pause the simulation. */
                void stop();
                void startRecording();
                void stopRecording();
                void showContextMenu(const QPoint &p);

                /** Reset simulation state to initial conditions. */
                void reset();
                /** Show the preferences dialog. */
                void preferences();
                void toggle_saved_camera_dialog();
                /** Show the about window. */
                void about();
                /** Request platform to open url to project website. */
                void projectWebsite();
                /** Set the inter-frame delay in milli-seconds. */
                void setInterFrameDelay(Uint delay);
                /** Show the preferences dialog. */
                void resetView();
                void updateOverlayWidget(MeshFile &data);
                bool runRecDlg();
                void toggleRecGui();
                void loadFile(QStringList files);

            private slots:
                /** Do any final connecting of components after rest of application is connected.*/
                void on_actionMesurement_Tool_triggered();
                void init();
                void openRecentFile();
                /** Update the enabled state of menu items.*/
                void updateMenusEnabledState();
                void resize640(){resize(computerWindowSizeForRenderSize(QSize(640,480)));}
                void resize720x480(){resize(computerWindowSizeForRenderSize(QSize(720,480)));}
                void resize720x576(){resize(computerWindowSizeForRenderSize(QSize(720,576)));}
                void resize960x540(){resize(computerWindowSizeForRenderSize(QSize(960,540)));}
                void showMeasureText(osg::Vec3 p,osg::Vec3 v);
            private:
                Q_DISABLE_COPY(EWSMainWindow)
                Ui::EWSMainWindowForm* _ui;
                SimulationState* _state;
                QFileDialog *qf;
                osg::ref_ptr<SceneRoot> _sceneRoot;
                QAction *separatorAct;
                QString curFile;
                QComboBox *overlay;
                enum { MaxRecentFiles = 5 };
                QAction *recentFileActs[MaxRecentFiles];
                bool firstRunRecord;
                SavedCameraWidget* _saved_camera_dialog;
                QMenu *conMenu;
            };
        }
    }
}

#endif // EWSMAINWINDOW_H
