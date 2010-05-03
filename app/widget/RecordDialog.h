#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class RecordDialog;
}

class RecordDialog : public QDialog {
    Q_OBJECT
public:
    RecordDialog(QWidget *parent = 0);
    ~RecordDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RecordDialog *m_ui;
};

#endif // RECORDDIALOG_H
