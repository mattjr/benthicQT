#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QtGui/QDialog>
#include "IVideoStreamer.h"

namespace Ui {
    class RecordDialog;
}

class RecordDialog : public QDialog {
    Q_OBJECT
public:
    RecordDialog(OSGVideoStreamer *iv,QWidget *parent = 0);
    ~RecordDialog();
    void setParams(int width,int height);
    int getResizeCmd();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RecordDialog *m_ui;
    OSGVideoStreamer *m_iv;

private slots:
    void on_toolButton_clicked();
};

#endif // RECORDDIALOG_H
