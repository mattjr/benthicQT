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

#include "BQTMainWindow.h"

#include "BarrierEditor.h"

#include "DrawableFactory.h"

#include "ui_BQTMainWindow.h"
#include <QMessageBox>
#include <QDesktopServices>
#include "BQTUtils.h"
#include "BQTVersion.h"
#include "BQTDefine.h"
//#include "AmplitudePlot.h"
//#include "PointSampler.h"
#include "Preferences.h"
#include "PositionHandler.h"
#include "UniformCallback.hpp"
#include <iostream>
namespace ews {
    namespace app {
        namespace widget {
            using ews::Uint;
            using namespace ews::app::drawable;
            using namespace ews::app::model;
            EWSMainWindow::EWSMainWindow(SimulationState* state, QWidget *parent) 
            : QMainWindow(parent), _ui(new Ui::EWSMainWindowForm), _state(state) {
                _ui->setupUi(this);
                qf=new QFileDialog();
                // Config actions
                //_ui->actionPause->setEnabled(false);
                _ui->actionOpen->setEnabled(true);
                

                
                // Config barrier editor.
                _ui->barrierEditor->setDataModel(&state->getMeshFiles());
                
                // Config detector editor.
//                _ui->detectorEditor->setDataModel(&state->getSamplers());
                
                // Config 3-D view.
                _sceneRoot = new SceneRoot(this);
                // renderer is an instance (the only one) of QOSGWidget
                _ui->renderer->setSceneData(_sceneRoot);
                        osg::Vec3 weights(6.24,0.79,0.3);
                        int num_shader_out=2;
                sseh=new ShaderSwitchEventHandler(weights,0,num_shader_out);

                _ui->renderer->addEventHandler(sseh);

                // Setup sync between model and renderer.
                QObject::connect(_state, SIGNAL(objectAdded(QObject&)), _sceneRoot, SLOT(addDrawableFor(QObject&)));
                QObject::connect(_state, SIGNAL(objectRemoved(QObject&)), _sceneRoot, SLOT(removeDrawableFor(QObject&)));
                QObject::connect(_ui->actionOpen, SIGNAL(triggered()), this, SLOT(openModel()));
                // Setup sync between samplers and plot.
                //QObject::connect(&_state->getSamplers(), SIGNAL(samplerAdded(int,PointSampler*)), 
                  //               _ui->amplitudePlot, SLOT(addSampleSource(int, PointSampler*)));
                //QObject::connect(&_state->getSamplers(), SIGNAL(samplerRemoved(int,PointSampler*)), 
                  //               _ui->amplitudePlot, SLOT(removeSampleSource(int, PointSampler*)));

                // Sync setup between 
                QObject::connect(&_state->getBarriers(), SIGNAL(sizeChanged(Uint)), this, SLOT(updateMenusEnabledState()));

            }
            
            EWSMainWindow::~EWSMainWindow() {
                _ui->renderer->setSceneData(NULL);
                delete _ui;
            }
            
            /** Perform post realization initialization() */
            void EWSMainWindow::init() {
                // Force synchronization between model and listeners.
                _state->emitSignalsForDefaults();
                
                // Make sure all consituents know we're getting ready to start.
                reset();
                
                // Fix menu state.
                updateMenusEnabledState();
                
                // Start simulation.
                start();
            }

            void EWSMainWindow::start() {
                _state->setPaused(false);
                //_ui->actionPause->setEnabled(true);
                //_ui->actionRun->setEnabled(false);
                
            }
            
            void EWSMainWindow::stop() {
                _state->setPaused(true);
                //_ui->actionPause->setEnabled(false);
               // _ui->actionRun->setEnabled(true);
            }
            
            void EWSMainWindow::reset() {
                _state->reset();

                _ui->renderer->homePosition();
               // _ui->amplitudePlot->reset();
                
                // If paused we fire a run action trigger
                // for convenience to start the simulation again
                // with the default state (and update the button enabled state).

                if(_state->isPaused()) {
                    //_ui->actionRun->trigger();
                }
                
                updateMenusEnabledState();       
            }
            
            /** Detect when we should perform post realization initialization() */
            bool EWSMainWindow::event(QEvent* event) {
                int retval = QWidget::event(event);

                if (event->type() == QEvent::Polish) {
                    init();
                }
                
                return retval;
            }
            
            void EWSMainWindow::updateMenusEnabledState() {
                _ui->actionAddBarrier->setEnabled(!_state->getBarriers().isFull());
                _ui->actionRemoveBarrier->setEnabled(_state->getBarriers().size() > 0);
            }
            
            Uint EWSMainWindow::getInterFrameDelay() const {
                return _ui->renderer->getInterFrameDelay();
            }
            
            void EWSMainWindow::setInterFrameDelay(Uint delayMS) {
                _ui->renderer->setInterFrameDelay(delayMS);
            }
            
            /** Prefs dialog. */
            void EWSMainWindow::preferences() {
                Preferences* p = new Preferences(this);
                p->show();
            }
            
            /** Show the about box. */
            void EWSMainWindow::about() {
                using ews::util::loadTextResource;
                QMessageBox::about(this, "About...", loadTextResource(":/text/about"));
            }
            /** Show reset view. */
            void EWSMainWindow::resetView   () {
                qDebug()<<"MadeIT";
               _ui->renderer->computeHomePosition();
            }
            /** Request the OS open the configured project site. */
            void EWSMainWindow::projectWebsite() {
                QUrl url(EWS_WEBSITE,QUrl::TolerantMode);
                qDebug() << "Opening" << url;
                QDesktopServices::openUrl(url);
            }

            void EWSMainWindow::openModel(){
                //Stop rendering or it causes freakout ?
                Uint fd=getInterFrameDelay();
                setInterFrameDelay(INT_MAX);
                ///

                QStringList files = qf->getOpenFileNames(this,"Choose Mesh","","Meshes (*.ive)");
                if (files.isEmpty())
                    return;
                QStringList list = files;
                QStringList::Iterator it = list.begin();
                QString first=list.front();
                while( it != list.end() ) {
                    qDebug() << "Opening" <<*it;
                    ++it;
                }


                _state->getMeshFiles().getPBarD()->setLabelText("Loading mesh "+first);
          //       _state->getMeshFiles().getPBarD()->reset();
                _state->getMeshFiles().getPBarD()->show();
                 sseh->setShaderOutUniform(_state->getMeshFiles().getShaderOutUniform());
                _state->getMeshFiles().setFileNames(files);
               _state->emitSignalsForLoad();

                _ui->renderer->computeHomePosition();
                     //Redo rendering delay
                 _state->getMeshFiles().getPBarD()->close();
                setInterFrameDelay(fd);

                qApp->processEvents();

            }


        }
    }
}

