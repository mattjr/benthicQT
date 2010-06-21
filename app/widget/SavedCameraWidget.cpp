#include "SavedCameraWidget.h"
#include "ui_SavedCameraWidget.h"
#include <stdio.h>

SavedCameraWidget::SavedCameraWidget(WorldWindManipulatorNew *manip,QWidget *parent) :
    QDockWidget(parent),_manip(manip),
    ui(new Ui::SavedCameraWidget)
{
    ui->setupUi(this);
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
    osg::Vec3 pos=_manip->getTargetCenter();
    char tmp[1024];
    sprintf(tmp,"Lat: %3.8f Long: %3.8f Z: %3.2f",pos[0],pos[1],pos[2]);
    QString posStr=tmp;
    ui->listWidget->addItem(posStr);
}
