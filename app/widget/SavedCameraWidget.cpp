#include "SavedCameraWidget.h"
#include "ui_SavedCameraWidget.h"

SavedCameraWidget::SavedCameraWidget(QWidget *parent) :
    QDockWidget(parent),
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
