#ifndef SAVEDCAMERAWIDGET_H
#define SAVEDCAMERAWIDGET_H

#include <QDockWidget>

namespace Ui {
    class SavedCameraWidget;
}

class SavedCameraWidget : public QDockWidget {
    Q_OBJECT
public:
    SavedCameraWidget(QWidget *parent = 0);
    ~SavedCameraWidget();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SavedCameraWidget *ui;
};

#endif // SAVEDCAMERAWIDGET_H
