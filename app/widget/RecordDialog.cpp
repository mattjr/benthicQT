#include "RecordDialog.h"
#include "ui_RecordDialog.h"

RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::RecordDialog)
{
    m_ui->setupUi(this);
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
