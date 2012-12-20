#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QDialog>
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
    enum{
        RESIZE_640_480,
        RESIZE_720_480,
        RESIZE_720_576,
        RESIZE_960_540
    };
protected:
    void changeEvent(QEvent *e);

private:
    Ui::RecordDialog *m_ui;
    OSGVideoStreamer *m_iv;
    int m_lastResizeEntry;

private slots:
    void on_codecCombo_currentIndexChanged(int index);
    void on_toolButton_clicked();
};

#endif // RECORDDIALOG_H
