#ifndef SAVEDCAMERAWIDGET_H
#define SAVEDCAMERAWIDGET_H

#include <QDockWidget>
#include "WWManipulator.hpp"

namespace Ui {
    class SavedCameraWidget;
}

class SavedCameraWidget : public QDockWidget {
    Q_OBJECT
public:
    SavedCameraWidget(WorldWindManipulatorNew *manip,QWidget *parent = 0);
    ~SavedCameraWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SavedCameraWidget *ui;
    WorldWindManipulatorNew *_manip;

private slots:
    void on_addSaved_clicked();
};

#endif // SAVEDCAMERAWIDGET_H
