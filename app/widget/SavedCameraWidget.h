#ifndef SAVEDCAMERAWIDGET_H
#define SAVEDCAMERAWIDGET_H

#include <QDockWidget>
#include "QOSGWidget.h"
#include <osg/AnimationPath>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
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
    void on_cameraStopButton_clicked();
    void on_cameraPauseButton_clicked();
    void on_cameraPlayButton_clicked();
    void on_deleteButton_clicked();
    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_2_clicked();
    void on_addSaved_clicked();
};
class SpinBoxDelegate : public QItemDelegate
 {
     Q_OBJECT

 public:
     SpinBoxDelegate(QObject *parent = 0);

     QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

     void setEditorData(QWidget *editor, const QModelIndex &index) const;
     void setModelData(QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;

     void updateEditorGeometry(QWidget *editor,
         const QStyleOptionViewItem &option, const QModelIndex &index) const;
 };
#endif // SAVEDCAMERAWIDGET_H
