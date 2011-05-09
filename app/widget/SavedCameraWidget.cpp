#include "SavedCameraWidget.h"
#include "ui_SavedCameraWidget.h"
#include <QtCore/QDebug>
 #include <QDoubleSpinBox>
#include <stdio.h>

SavedCameraWidget::SavedCameraWidget(ews::app::widget::QOSGWidget *qosgWidget,QWidget *parent) :
    QDockWidget(parent),_qosgWidget(qosgWidget),
    ui(new Ui::SavedCameraWidget),_maxTime(0.0)
{
    ui->setupUi(this);
    _ap = _qosgWidget->_ap;
}

SavedCameraWidget::~SavedCameraWidget()
{
    delete ui;
}

void SavedCameraWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SavedCameraWidget::on_addSaved_clicked()
{
    WorldWindManipulatorNew *ctrl;
    if(ctrl = dynamic_cast<WorldWindManipulatorNew*> (_qosgWidget->getCameraManipulator())) {

        osg::Vec3 pos=ctrl->getTargetCenter();
        MyAnimationPath::ControlPoint cp(ctrl->getTargetCenter(),ctrl->getTargetOrientation(),
                                         ctrl->getTargetDistance(),ctrl->getTargetTilt());
        _ap->insert(_maxTime++,cp);
    }
    updateList();
}

void SavedCameraWidget::updateList(){
    MyAnimationPath::TimeControlPointMap &tcpm=_ap->getTimeControlPointMap();
    MyAnimationPath::TimeControlPointMap::iterator iter;
    ui->listWidget->clear();

    for(iter=tcpm.begin(); iter != tcpm.end(); iter++){
           QListWidgetItem *qtItem = new QListWidgetItem();
           char tmp[1024];
           osg::Vec3 pos=iter->second.getCenter();
           sprintf(tmp,"Time %.1f X: %3.8f Y: %3.8f Z: %3.2f",iter->first,pos[0],pos[1],pos[2]);
           QString posStr=tmp;
           qtItem->setText(posStr);
           QDoubleSpinBox *spinbox=new QDoubleSpinBox();
           spinbox->setValue(iter->first);
           ui->listWidget->addItem(qtItem);
             //ui->listWidget->addItem(spinbox);
    }
}

void SavedCameraWidget::on_pushButton_2_clicked()
{
    _qosgWidget->switchToFromAniManip();

}

void SavedCameraWidget::on_listWidget_currentRowChanged(int currentRow)
{
    double timeProxy=currentRow;
    osg::Matrix mat;
    if (_ap->getTimeControlPointMap().count(timeProxy)){
        MyAnimationPath::ControlPoint cp=_ap->getTimeControlPointMap()[timeProxy];

        _ap->getMatrix(timeProxy,mat);
        WorldWindManipulatorNew *ctrl;
        if(ctrl = dynamic_cast<WorldWindManipulatorNew*> (_qosgWidget->getCameraManipulator())) {
            ctrl->moveTo(cp.getCenter(),cp.getOrientation(),cp.getDistance(),cp.getTilt());
      /*              ctrl->setCenter(cp.getCenter());
            ctrl->setTargetOrientation(cp.getOrientation());
            ctrl->setTargetDistance(cp.getDistance());
            ctrl->setTargetTilt(cp.getTilt());*/
        }
    }
  //  qDebug() << "Current row " << currentRow <<"\n";
}

void SavedCameraWidget::on_deleteButton_clicked()
{

}

void SavedCameraWidget::on_cameraPlayButton_clicked()
{
    _qosgWidget->switchToFromAniManip(ews::app::widget::QOSGWidget::ANIM_MANIP);

}

void SavedCameraWidget::on_cameraPauseButton_clicked()
{

                _qosgWidget->pauseAnim();

   // _qosgWidget->_animationManip->s
//_ap
}

void SavedCameraWidget::on_cameraStopButton_clicked()
{
    _qosgWidget->switchToFromAniManip(ews::app::widget::QOSGWidget::WW_MANIP);
 //   _qosgWidget->switchToFromAniManip(ews::app::widget::QOSGWidget::ANIM_MANIP);

}
