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
#include <iostream>
#include "GLPreCompile.h"
#include "SavedCameraWidget.h"
#include "RecordDialog.h"
#include "ScreenTools.h"
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
                firstRunRecord=false;
                // Config actions
                //_ui->actionPause->setEnabled(false);
                _ui->actionOpen->setEnabled(true);
                setInterFrameDelay(15);

                _ui->menuItems->setEnabled(OSGVideoStreamer::enabled);
                // Config barrier editor.
                _ui->barrierEditor->setDataModel(&state->getMeshFiles());
                
                // Config detector editor.
                //                _ui->detectorEditor->setDataModel(&state->getSamplers());
                
                // Config 3-D view.
                _sceneRoot = new SceneRoot(this);
                // renderer is an instance (the only one) of QOSGWidget
                _ui->renderer->setSceneData(_sceneRoot);
                _ui->renderer->getDatabasePager()->setDrawablePolicy(osgDB::DatabasePager::DO_NOT_MODIFY_DRAWABLE_SETTINGS);
                _ui->renderer->getDatabasePager()->setDoPreCompile(true);

                osg::Vec3 weights(6.24,0.79,0.3);
                int num_shader_out=2;
                overlay=new QComboBox;
                _ui->mainToolBar->addWidget(overlay);
                datausedCombo =new QComboBox;
                _ui->mainToolBar->addWidget(datausedCombo);
                colormapCombo=new QComboBox;
                _ui->mainToolBar->addWidget(colormapCombo);
                // Setup sync between model and renderer.
                QObject::connect(_state, SIGNAL(objectAdded(QObject&)), _sceneRoot, SLOT(addDrawableFor(QObject&)));
                QObject::connect(_state, SIGNAL(objectRemoved(QObject&)), _sceneRoot, SLOT(removeDrawableFor(QObject&)));
                QObject::connect(_ui->actionOpen, SIGNAL(triggered()), this, SLOT(openModel()));
                QObject::connect(_ui->actionStart_Recording, SIGNAL(triggered()), this,SLOT(startRecording()));
                QObject::connect(_ui->actionStop_Recording, SIGNAL(triggered()), this,SLOT(stopRecording()));
                QObject::connect(_ui->actionSetup_Recording, SIGNAL(triggered()), this,SLOT(runRecDlg()));

                QObject::connect(_ui->actionSet_to_640x480, SIGNAL(triggered()), this, SLOT(resize640()));
                QObject::connect(_ui->actionSet_to_720x480, SIGNAL(triggered()), this, SLOT(resize720x480()));
                QObject::connect(_ui->actionSet_to_720x576, SIGNAL(triggered()), this, SLOT(resize720x576()));
                QObject::connect(_ui->actionSet_to_960x540, SIGNAL(triggered()), this, SLOT(resize960x540()));
                QObject::connect(overlay, SIGNAL(currentIndexChanged(int)), _ui->barrierEditor, SLOT(changeOverlay(int)));
                QObject::connect(datausedCombo, SIGNAL(currentIndexChanged(int)), _ui->barrierEditor, SLOT(changeDataUsed(int)));
                QObject::connect(colormapCombo, SIGNAL(currentIndexChanged(int)), _ui->barrierEditor, SLOT(changeColorMap(int)));

                QObject::connect(_ui->actionSavedCamera, SIGNAL(triggered()), this, SLOT(toggle_saved_camera_dialog()));
                _ui->actionSavedCamera->setEnabled(true);

             //   QObject::connect(&_state->getMeshFiles(), SIGNAL(measureResults(osg::Vec3,osg::Vec3)), this, SLOT(showMeasureText(osg::Vec3,osg::Vec3)));

                // Setup sync between samplers and plot.
                //QObject::connect(&_state->getSamplers(), SIGNAL(samplerAdded(int,PointSampler*)), 
                //               _ui->amplitudePlot, SLOT(addSampleSource(int, PointSampler*)));
                //QObject::connect(&_state->getSamplers(), SIGNAL(samplerRemoved(int,PointSampler*)), 
                //               _ui->amplitudePlot, SLOT(removeSampleSource(int, PointSampler*)));

                // Sync setup between 
                QObject::connect(&_state->getBarriers(), SIGNAL(sizeChanged(Uint)), this, SLOT(updateMenusEnabledState()));

                for (int i = 0; i < MaxRecentFiles; ++i) {
                    recentFileActs[i] = new QAction(this);
                    recentFileActs[i]->setVisible(false);
                    QObject::connect(recentFileActs[i], SIGNAL(triggered()),
                                     this, SLOT(openRecentFile()));
                }
                separatorAct = _ui->menuFile->addSeparator();
                _ui->actionStop_Recording->setEnabled(false);
                _saved_camera_dialog = new SavedCameraWidget(_ui->renderer);
                updateRecentFileActions();
                setWindowTitle(tr("BenthicQT"));
                if(!OSGVideoStreamer::enabled){
                    _ui->actionStart_Recording->setEnabled(false);
                    _ui->actionStop_Recording->setEnabled(false);
                    _ui->menuItems->setTitle("");
                    _ui->menuItems->setVisible(false);
                    _ui->actionSetup_Recording->setEnabled(false);

                }
               // _ui->renderer->
                _ui->renderer->installEventFilter(this);
              //  installEventFilter(this);
                      //  setContextMenuPolicy(Qt::ActionsContextMenu);
                //_ui->renderer->
                        //addAction(_ui->actionOpen);
                conMenu=new QMenu;
               conMenu->addAction(_ui->actionOpen_Images);
               conMenu->addAction(_ui->actionCopyImageNames);

                if(_ui->renderer->movieCallback && OSGVideoStreamer::enabled){
                    OSGVideoStreamer *iv=_ui->renderer->movieCallback->getRecorder(_ui->renderer->_gw);
                    if(!iv->getEncoderNames().size()){
                       QErrorMessage errorMessageDialog(this);
                        errorMessageDialog.showMessage("No codec avalible diabling movie recording\n");
                        errorMessageDialog.exec();
                        _ui->actionStart_Recording->setEnabled(false);
                        _ui->actionSetup_Recording->setEnabled(false);
                    }
                }
                for (int i = 0; i < MaxRecentFiles; ++i)
                    _ui->menuFile->addAction(recentFileActs[i]);
                _ui->menuFile->addSeparator();
            }
            
            EWSMainWindow::~EWSMainWindow() {
                _ui->renderer->setSceneData(NULL);
                _saved_camera_dialog->close();
                delete _saved_camera_dialog;
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
            QSize EWSMainWindow::computerWindowSizeForRenderSize(QSize target){
                QSize fullWin=size();
                QSize diff =fullWin-_ui->renderer->size();
                return diff+target;
            }
            void EWSMainWindow::showContextMenu(const QPoint &p)
            {
                conMenu->popup(p);
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

            bool EWSMainWindow::eventFilter(QObject * watched, QEvent * event){
                if(event->type() == QEvent::MouseButtonPress){
                    if(((QMouseEvent*)event)->button() == Qt::MidButton){
                        showContextMenu(((QMouseEvent*)event)->globalPos());
                        return true;
                    }
                }
                return QObject::eventFilter(watched, event);
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
                //_ui->actionAddBarrier->setEnabled(!_state->getBarriers().isFull());
                //_ui->actionRemoveBarrier->setEnabled(_state->getBarriers().size() > 0);
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
            
            /** Prefs dialog. */
            void EWSMainWindow::toggle_saved_camera_dialog() {


               //s->setFloating(true);
                if(_ui->actionSavedCamera->isChecked()){
                    addDockWidget(Qt::RightDockWidgetArea, _saved_camera_dialog);
                    _saved_camera_dialog->show();
                }
                else
                    removeDockWidget(_saved_camera_dialog);


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
                QFileDialog::Options options;// = QFileDialog::DontUseNativeDialog;
                QString selectedFilter;

                QStringList files = qf->getOpenFileNames(this,"Choose Mesh","","Meshes (*.ive *.osgb *.osga)",&selectedFilter,options);

                loadFile( files);
                setInterFrameDelay(fd);
                qApp->processEvents();

            }
            void EWSMainWindow::loadFile(QStringList files){
                if (files.isEmpty())
                    return;
                QStringList list = files;
                QStringList::Iterator it = list.begin();
                QString first=list.front();
                while( it != list.end() ) {
                    qDebug() << "Opening" <<*it;
                    ++it;
                }
                QApplication::setOverrideCursor(Qt::WaitCursor);


                _state->getMeshFiles().getPBarD()->setLabelText("Loading mesh "+first);
                //       _state->getMeshFiles().getPBarD()->reset();
                _state->getMeshFiles().getPBarD()->show();
                //  sseh->setShaderOutUniform(_state->getMeshFiles().getShaderOutUniform());
                _state->getMeshFiles().setRenderer(_ui->renderer);

                _state->getMeshFiles().setFileNames(files);

                _state->emitSignalsForLoad();

                _ui->renderer->computeHomePosition();
                _state->getMeshFiles().getPBarD()->setLabelText("Compiling shaders. Please Wait.");
                _state->getMeshFiles().getPBarD()->setRange(0,0);
                qApp->processEvents();
                CompileObjects compile;
                compile.setState( _ui->renderer->getState());
                _ui->renderer->getSceneData()->accept(compile);

                _ui->renderer->paintGL();
                qApp->processEvents();
                //Redo rendering delay
                _state->getMeshFiles().getPBarD()->close();
                updateOverlayWidget(_state->getMeshFiles());
                QApplication::restoreOverrideCursor();
                setCurrentFile(first);

                QObject::connect(_ui->actionOpen_Images, SIGNAL(triggered()), &_state->getMeshFiles(), SLOT(openCurrentImage()));
                QObject::connect(_ui->actionToggleMinimap, SIGNAL(triggered(bool)), &_state->getMeshFiles(), SLOT(switchMinimap(bool)));

                QObject::connect( &_state->getMeshFiles(), SIGNAL(measureResults(osg::Vec3,osg::Vec3)),_ui->barrierEditor, SLOT(displayMeasure(osg::Vec3,osg::Vec3)));
                QObject::connect(_ui->actionOpen_Images, SIGNAL(triggered()), &_state->getMeshFiles(), SLOT(openCurrentImage()));
                QObject::connect(_ui->actionCopy_Images_Names, SIGNAL(triggered()), &_state->getMeshFiles(), SLOT(copyCurrentImageClipboard()));



            }
            void EWSMainWindow::setCurrentFile(const QString &fileName)
            {
                curFile = fileName;
                if (curFile.isEmpty())
                    setWindowTitle(tr("BenthicQT"));
                else
                    setWindowTitle(tr("%1 - %2").arg(strippedName(curFile))
                                   .arg(tr("BenthicQT")));

                QSettings settings("ACFR", "BenthicQT Viewer");
                QStringList files = settings.value("recentFileList").toStringList();
                files.removeAll(fileName);
                files.prepend(fileName);
                while (files.size() > MaxRecentFiles)
                    files.removeLast();

                settings.setValue("recentFileList", files);

                foreach (QWidget *widget, QApplication::topLevelWidgets()) {
                    EWSMainWindow *mainWin = qobject_cast<EWSMainWindow *>(widget);
                    if (mainWin)
                        mainWin->updateRecentFileActions();
                }
            }

            void EWSMainWindow::updateRecentFileActions()
            {
                QSettings settings("ACFR", "BenthicQT Viewer");
                QStringList files = settings.value("recentFileList").toStringList();

                int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

                for (int i = 0; i < numRecentFiles; ++i) {
                    QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
                    recentFileActs[i]->setText(text);
                    recentFileActs[i]->setData(files[i]);
                    recentFileActs[i]->setVisible(true);
                }
                for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
                    recentFileActs[j]->setVisible(false);

                separatorAct->setVisible(numRecentFiles > 0);
            }
            void EWSMainWindow::openRecentFile()
            {
                QAction *action = qobject_cast<QAction *>(sender());
                if (action){
                    QStringList files;
                    files.push_back(action->data().toString());
                    loadFile(files);
                }
            }
            QString EWSMainWindow::strippedName(const QString &fullFileName)
            {
                QFileInfo p(fullFileName);
                QStringList dirs = p.path().split( "/");
                int index = max( 0, (int)dirs.size()-1 );
                if( dirs[index] == "mesh")
                    index = max(0, (int)dirs.size()-2);
                return dirs[index]+" ("+p.fileName()+")";
            }
            void EWSMainWindow::updateOverlayWidget(MeshFile &data){
                overlay->clear();
                datausedCombo->clear();
                colormapCombo->clear();

                for(int i=0; i< data.getShaderNames().size(); i++)
                    overlay->addItem(data.getShaderNames()[i].c_str());
                for(int i=0; i< data.getColorMapNames().size(); i++)
                    colormapCombo->addItem(data.getColorMapNames()[i].c_str());
                for(int i=0; i< data.getDataUsedNames().size(); i++)
                    datausedCombo->addItem(data.getDataUsedNames()[i].c_str());
                /*   else{
                       char tmp[255];
                       sprintf(tmp,"Aux %d",i);
                       QString qtmp=tmp;
                       overlay->addItem(qtmp);
                       //qDebug() << "Adding " <<qtmp;
                   }

               }*/
            }
             bool EWSMainWindow::runRecDlg(){
                 if(!_ui->renderer->movieCallback)
                     return false;
                 OSGVideoStreamer *iv=_ui->renderer->movieCallback->getRecorder(_ui->renderer->_gw);
                 RecordDialog recdlg(iv);
                 if(recdlg.exec()){
                     int resizeCmd=recdlg.getResizeCmd();
                     if(resizeCmd == RecordDialog::RESIZE_640_480)
                         resize640();
                     else if(resizeCmd == RecordDialog::RESIZE_720_480)
                         resize720x480();
                     else if(resizeCmd == RecordDialog::RESIZE_720_576)
                         resize720x576();
                     else if(resizeCmd == RecordDialog::RESIZE_960_540)
                         resize960x540();


                     QSize size=_ui->renderer->size();

                     recdlg.setParams(size.width(),size.height());
                     firstRunRecord=true;
                     return true;
                 }
                 else return false;
             }
             void EWSMainWindow::startRecording(){

                 if(!firstRunRecord)
                     runRecDlg();
                 if(firstRunRecord){
                     _ui->renderer->startRecording();
                     toggleRecGui();
                 }
             }
              void EWSMainWindow::stopRecording(){
                   toggleRecGui();
                  _ui->renderer->stopRecording();
              }
              void EWSMainWindow::toggleRecGui(){
                    _ui->actionStart_Recording->setEnabled(!_ui->actionStart_Recording->isEnabled());
                    _ui->actionStop_Recording->setEnabled(!_ui->actionStart_Recording->isEnabled());
              }
              void EWSMainWindow::showMeasureText(osg::Vec3 p,osg::Vec3 v){

              }
              void EWSMainWindow::on_actionMesurement_Tool_triggered()
              {
                  measuring_tool_on=_ui->actionMesurement_Tool->isChecked();
              }


         }
    }
}


