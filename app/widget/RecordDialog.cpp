#include "RecordDialog.h"
#include "ui_RecordDialog.h"
#include <QtCore/QDir>
#include <QtGui/QFileDialog>
RecordDialog::RecordDialog(OSGVideoStreamer *iv,QWidget *parent) :
    QDialog(parent),m_iv(iv),
    m_ui(new Ui::RecordDialog)
{
    m_ui->setupUi(this);
    m_ui->saveDir->setText( QDir::homePath());
    if(m_iv){
        for(int i=0; i < iv->getEncoderNames().size(); i++)
            m_ui->codecCombo->addItem(QString(iv->getEncoderNames()[i].c_str()));
    }else
        m_ui->codecCombo->addItem(QString("No Encoders"));
 m_ui->codecCombo->setCurrentIndex(0);
 m_ui->resizeCombo->addItem(QString("Resize 640x480"));
 m_ui->resizeCombo->addItem(QString("Resize 720x480"));
 m_ui->resizeCombo->addItem(QString("Resize 720x576"));
 m_ui->resizeCombo->addItem(QString("Resize 960x540"));
 m_ui->resizeCombo->addItem(QString("No Resize"));

 m_ui->resizeCombo->setCurrentIndex(0);
 m_lastResizeEntry=0;


}
void RecordDialog::setParams(int width,int height){
#ifdef USE_FFMPEG

    if(m_iv){
        m_iv->SetupVideo(m_ui->saveDir->text().toStdString(),
                         m_ui->baseName->text().toStdString(),
                         m_ui->codecCombo->currentIndex(),
                         width,
                         height,
                         m_ui->bitRate->value()*1000);
    }
#endif

}
RecordDialog::~RecordDialog()
{
    delete m_ui;
}
int RecordDialog::getResizeCmd(){
    return m_ui->resizeCombo->currentIndex();
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

void RecordDialog::on_toolButton_clicked()
{
QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
if(dir.size())
     m_ui->saveDir->setText(dir);

}







void RecordDialog::on_codecCombo_currentIndexChanged(int index)
{
    int dvComboEntry=-1;

     if(m_iv){
        if(m_iv->dvComboEntry >= 0 && index==m_iv->dvComboEntry){
            m_ui->resizeCombo->setDisabled(true);
            m_ui->bitRate->setDisabled(true);
            m_lastResizeEntry=m_ui->resizeCombo->currentIndex();
            m_ui->resizeCombo->setCurrentIndex(RecordDialog::RESIZE_720_576);
        }else if(!m_ui->resizeCombo->isEnabled()){
            m_ui->resizeCombo->setCurrentIndex(m_lastResizeEntry);
            m_ui->resizeCombo->setDisabled(false);
            m_ui->bitRate->setDisabled(false);

        }
    }
}
