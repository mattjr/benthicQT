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

#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include <QString>
#include <QErrorMessage>
#include <osg/Notify>
#include <iostream>
#include <sstream>
#include "BQTMainWindow.h"
#include "BQTVersion.h"

#if defined(WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define __WINLOG
#endif


inline void logMessage(const char* typeStr, const char* msg) {
    std::ostringstream oss;
    oss << typeStr << ": " << msg << std::endl;
    std::cerr << oss.str();
#if defined(__WINLOG)
    OutputDebugStringA(oss.str().c_str());
#endif
}

static QtMsgHandler defMsgHandler = NULL;

/** Message handler for Qt logging system. */
void messageHandler(QtMsgType type, const char *msg) {
    switch (type) {
        case QtDebugMsg:
            logMessage("Debug", msg);
            break;
        case QtWarningMsg:
            logMessage("Warning", msg);
            break;
        case QtCriticalMsg:
            logMessage("Critical", msg);
            break;
        case QtFatalMsg:
            logMessage("Fatal", msg);
            break;
    }
    
    if(type != QtDebugMsg && defMsgHandler) {
        QWidget* win = QApplication::activeWindow();
        QMainWindow* main = qobject_cast<QMainWindow*>(win);
        if(main) {
            main->lower();
        }
        defMsgHandler(type, msg);
    }
}

/** Application launch functionn. */
int main(int argc, char *argv[]) {
    //using namespace ews::app::model;
    using namespace ews::app::widget;

#if defined(QT_DEBUG)                
   // osg::setNotifyLevel(osg::INFO);
#endif                
    
    // To see Qt object dumps on macos, run with the environment variable
    // "DYLD_IMAGE_SUFFIX" set to "_debug".
    
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);
    QApplication::setOrganizationName(EWS_ORGANIZATION_NAME);
    QApplication::setOrganizationDomain(EWS_BUNDLE_ID);
    QApplication::setApplicationVersion(EWS_VERSION);
    QApplication::setApplicationName(EWS_APP_NAME);
    
    // First register the dialog error handler, then
    // get the function pointer to it by passing zero to the handler installer
    // then install our own.
    QErrorMessage::qtHandler();
    defMsgHandler = qInstallMsgHandler(0);
    qInstallMsgHandler(messageHandler);
    
    QSplashScreen splash;
    QPixmap img(":/images/splash");
    
    if(img.isNull()) {
       qWarning() << "Couldn't load splash image";
    }
    
    splash.setPixmap(img);
    splash.showMessage(QObject::tr("Starting up...."));
    splash.show();

    
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    SimulationState state;
    state.setObjectName("root");

    EWSMainWindow w(&state);
    w.setWindowTitle(EWS_APP_NAME);
#if defined(__APPLE__)
    w.setWindowIcon(QIcon(":/images/appicon"));
#else    
    QApplication::setWindowIcon(QIcon(":/images/appicon"));    
#endif    


    w.show();
    
    splash.finish(&w);


    return a.exec();
}
