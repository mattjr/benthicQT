#ifndef SAVEDCAMERAWIDGET_H
#define SAVEDCAMERAWIDGET_H

#include <QDockWidget>
#include "QOSGWidget.h"
#include <osg/AnimationPath>

namespace Ui {
    class SavedCameraWidget;
}

class SavedCameraWidget : public QDockWidget {
    Q_OBJECT
public:
    SavedCameraWidget(ews::app::widget::QOSGWidget *qosgWidget,QWidget *parent = 0);
    ~SavedCameraWidget();

protected:
    void changeEvent(QEvent *e);
    void updateList();

private:
    Ui::SavedCameraWidget *ui;
    ews::app::widget::QOSGWidget *_qosgWidget;
    MyAnimationPath *_ap;
    double _maxTime;


private slots:
    void on_deleteButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_2_clicked();
    void on_addSaved_clicked();
};

#endif // SAVEDCAMERAWIDGET_H
