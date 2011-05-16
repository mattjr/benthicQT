#include "SavedCameraWidget.h"
#include "ui_SavedCameraWidget.h"
#include <QtCore/QDebug>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <stdio.h>
#include <QTableWidget>
#include "AnimationPathPlayer.hpp"
#include <QFileDialog>
SpinBoxDelegate::SpinBoxDelegate(MyAnimationPath::TimeControlPointMap &tcpm,QObject *parent)
    :_tcpm(tcpm), QItemDelegate(parent)
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

    MyAnimationPath::TimeControlPointMap::iterator iter;
    int row=0;
    double updateDiff=0;
     MyAnimationPath::TimeControlPointMap newMap;
    for(iter=_tcpm.begin(); iter != _tcpm.end(); iter++){
        if(row < index.row()){
            double orig=iter->first;

            MyAnimationPath::ControlPoint cp=iter->second;
            newMap.insert(std::make_pair<double,MyAnimationPath::ControlPoint>(orig,cp));
        }else if(row == index.row()){
            double orig=iter->first;
            updateDiff=value-orig;
            MyAnimationPath::ControlPoint cp=iter->second;
            newMap.insert(std::make_pair<double,MyAnimationPath::ControlPoint>((double)value,cp));
        }else if(row > index.row()){
            double orig=iter->first;
            MyAnimationPath::ControlPoint cp=iter->second;
            newMap.insert(std::make_pair<double,MyAnimationPath::ControlPoint>(orig+updateDiff,cp));
        }
        row++;
    }
    _tcpm=newMap;

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
        MyAnimationPath::TimeControlPointMap &tcpm=_ap->getTimeControlPointMap();
        MyAnimationPath::TimeControlPointMap::iterator iter;

        for(iter=tcpm.begin(); iter != tcpm.end(); iter++){
            if(iter->first > _maxTime)
                _maxTime=iter->first;
        }
        _maxTime++;
        _ap->insert(_maxTime,cp);
    }
    updateList();
}

void SavedCameraWidget::updateList(){
    MyAnimationPath::TimeControlPointMap &tcpm=_ap->getTimeControlPointMap();
    MyAnimationPath::TimeControlPointMap::iterator iter;
    //ui->tableWidget->clear();
    SpinBoxDelegate* delegate = new SpinBoxDelegate(tcpm,ui->tableWidget);
    ui->tableWidget->setItemDelegate(delegate);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(tcpm.size());

    int row=0;
    for(iter=tcpm.begin(); iter != tcpm.end(); iter++){
        QTableWidgetItem *posItem[3];
        for(int i=0; i <3; i++)
            posItem[i]= new QTableWidgetItem();

        QTableWidgetItem *timeItem = new QTableWidgetItem();

        char tmp[1024];
        osg::Vec3 pos=iter->second.getCenter();

        //      posItem->setFlags (posItem->flags () | ~Qt::ItemIsEditable);
        sprintf(tmp,"%f",iter->first);
        QString timeStr=tmp;
        timeItem->setText(timeStr);
        timeItem->setFlags (timeItem->flags () | Qt::ItemIsEditable);

        //QModelIndex index = model.index(row, column, QModelIndex());
        //        model.setData(index, QVariant((row+1) * (column+1)));
        for(int i=0; i < 3; i++){
            sprintf(tmp,"%3.8f",pos[i]);
            QString posStr=tmp;
            posItem[i]->setText(posStr);
            ui->tableWidget->setItem(row,1+i,posItem[i]);

        }
        ui->tableWidget->setItem(row,0,timeItem);
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

void SavedCameraWidget::on_loadButton_clicked()
{
    QFileDialog::Options options;
       /*  if (!native->isChecked())
             options |= QFileDialog::DontUseNativeDialog;*/
         QString selectedFilter;
         QString fileName = QFileDialog::getOpenFileName(this,
                                     tr("QFileDialog::getOpenFileName()"),tr(""),
                                    // openFileNameLabel->text(),
                                     tr("All Files (*);;Text Files (*.txt)"),
                                     &selectedFilter,
                                     options);
         if (!fileName.isEmpty()){
             std::ifstream fin(fileName.toAscii());
            _ap->read(fin);
            updateList();
         }
}

void SavedCameraWidget::on_saveButton_clicked()
{
    QFileDialog::Options options;

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Control Points"),tr(""),
                                                    //saveFileNameLabel->text(),
                                                    tr("All Files (*);;Text Files (*.txt)"),
                                                    &selectedFilter,
                                                    options);
    if (!fileName.isEmpty()){
     std::ofstream fout(fileName.toAscii());
     if(fout)
         _ap->write(fout);
     else
         std::cerr<<"can't open file " <<"\n";
    }
        //->setText(fileName);
}
