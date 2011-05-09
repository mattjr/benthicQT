#include "SavedCameraWidget.h"
#include "ui_SavedCameraWidget.h"
#include <QtCore/QDebug>
 #include <QDoubleSpinBox>
#include <QLineEdit>
#include <stdio.h>
#include <QTableWidget>
SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
     : QItemDelegate(parent)
 {
 }

 QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
     QSpinBox *editor = new QSpinBox(parent);
     editor->setMinimum(0);
     editor->setMaximum(100);

     return editor;
 }

 void SpinBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     int value = index.model()->data(index, Qt::EditRole).toInt();

     QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
     spinBox->setValue(value);
 }

 void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
     spinBox->interpretText();
     int value = spinBox->value();

     model->setData(index, value, Qt::EditRole);
 }

 void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }
SavedCameraWidget::SavedCameraWidget(ews::app::widget::QOSGWidget *qosgWidget,QWidget *parent) :
    QDockWidget(parent),_qosgWidget(qosgWidget),
    ui(new Ui::SavedCameraWidget),_maxTime(0.0)
{
    ui->setupUi(this);
    _ap = _qosgWidget->_ap;
}

SavedCameraWidget::~SavedCameraWidget()
{
    delete ui;
}

void SavedCameraWidget::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SavedCameraWidget::on_addSaved_clicked()
{
    WorldWindManipulatorNew *ctrl;
    if(ctrl = dynamic_cast<WorldWindManipulatorNew*> (_qosgWidget->getCameraManipulator())) {

        osg::Vec3 pos=ctrl->getTargetCenter();
        MyAnimationPath::ControlPoint cp(ctrl->getTargetCenter(),ctrl->getTargetOrientation(),
                                         ctrl->getTargetDistance(),ctrl->getTargetTilt());
        _ap->insert(_maxTime++,cp);
    }
    updateList();
}

void SavedCameraWidget::updateList(){
    MyAnimationPath::TimeControlPointMap &tcpm=_ap->getTimeControlPointMap();
    MyAnimationPath::TimeControlPointMap::iterator iter;
    //ui->tableWidget->clear();
    SpinBoxDelegate* delegate = new SpinBoxDelegate(ui->tableWidget);
    ui->tableWidget->setItemDelegate(new SpinBoxDelegate);
       ui->tableWidget->setColumnCount(2);
       ui->tableWidget->setRowCount(tcpm.size());

int row=0;
    for(iter=tcpm.begin(); iter != tcpm.end(); iter++){
           QTableWidgetItem *posItem = new QTableWidgetItem();
           QTableWidgetItem *timeItem = new QTableWidgetItem();

           char tmp[1024];
           osg::Vec3 pos=iter->second.getCenter();
           sprintf(tmp,"X: %3.8f Y: %3.8f Z: %3.2f",pos[0],pos[1],pos[2]);
           QString posStr=tmp;
           posItem->setText(posStr);
     //      posItem->setFlags (posItem->flags () | ~Qt::ItemIsEditable);
           sprintf(tmp,"Time %f",iter->first);
           QString timeStr=tmp;
           timeItem->setText(timeStr);
           timeItem->setFlags (timeItem->flags () | Qt::ItemIsEditable);

           //QModelIndex index = model.index(row, column, QModelIndex());
             //        model.setData(index, QVariant((row+1) * (column+1)));
           ui->tableWidget->setItem(row,0,posItem);
           ui->tableWidget->setItem(row,1,timeItem);
           row++;

    }
}

void SavedCameraWidget::on_pushButton_2_clicked()
{
    _qosgWidget->switchToFromAniManip();

}

void SavedCameraWidget::on_listWidget_currentRowChanged(int currentRow)
{
    double timeProxy=currentRow;
    osg::Matrix mat;
    if (_ap->getTimeControlPointMap().count(timeProxy)){
        MyAnimationPath::ControlPoint cp=_ap->getTimeControlPointMap()[timeProxy];

        _ap->getMatrix(timeProxy,mat);
        WorldWindManipulatorNew *ctrl;
        if(ctrl = dynamic_cast<WorldWindManipulatorNew*> (_qosgWidget->getCameraManipulator())) {
            ctrl->moveTo(cp.getCenter(),cp.getOrientation(),cp.getDistance(),cp.getTilt());
      /*              ctrl->setCenter(cp.getCenter());
            ctrl->setTargetOrientation(cp.getOrientation());
            ctrl->setTargetDistance(cp.getDistance());
            ctrl->setTargetTilt(cp.getTilt());*/
        }
    }
  //  qDebug() << "Current row " << currentRow <<"\n";
}

void SavedCameraWidget::on_deleteButton_clicked()
{

}

void SavedCameraWidget::on_cameraPlayButton_clicked()
{
    _qosgWidget->switchToFromAniManip(ews::app::widget::QOSGWidget::ANIM_MANIP);

}

void SavedCameraWidget::on_cameraPauseButton_clicked()
{

                _qosgWidget->pauseAnim();

   // _qosgWidget->_animationManip->s
//_ap
}

void SavedCameraWidget::on_cameraStopButton_clicked()
{
    _qosgWidget->switchToFromAniManip(ews::app::widget::QOSGWidget::WW_MANIP);
 //   _qosgWidget->switchToFromAniManip(ews::app::widget::QOSGWidget::ANIM_MANIP);

}
