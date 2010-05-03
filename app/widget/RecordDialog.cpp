#include "RecordDialog.h"
#include "ui_RecordDialog.h"
#include <QtCore/QDir>
RecordDialog::RecordDialog(OSGVideoStreamer *iv,QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::RecordDialog)
{
    m_ui->setupUi(this);
    m_ui->saveDir->setText( QDir::homePath());
    if(iv){
        for(int i=0; i < iv->getEncoderNames().size(); i++)
            m_ui->codecCombo->addItem(QString(iv->getEncoderNames()[i].c_str()));
    }else
        m_ui->codecCombo->addItem(QString("No Encoders"));
 m_ui->codecCombo->setCurrentIndex(0);
 m_ui->resizeCombo->addItem(QString("Resize 640x480"));
 m_ui->resizeCombo->addItem(QString("Resize 720x480"));
 m_ui->resizeCombo->addItem(QString("No Resize"));

 m_ui->resizeCombo->setCurrentIndex(0);



}

RecordDialog::~RecordDialog()
{
    delete m_ui;
}

void RecordDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
