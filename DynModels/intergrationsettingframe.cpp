#include "intergrationsettingframe.h"
#include "ui_intergrationsettingframe.h"

IntergrationSettingFrame::IntergrationSettingFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::IntergrationSettingFrame)
{
    ui->setupUi(this);
}

IntergrationSettingFrame::~IntergrationSettingFrame()
{
    delete ui;
}

qreal IntergrationSettingFrame::getStart() const
{
    return ui->dsbStart->value();
}

qreal IntergrationSettingFrame::getEnd() const
{
    return ui->dsbEnd->value();
}

qreal IntergrationSettingFrame::getStep() const
{
    return ui->dsbStep->value();
}

quint8 IntergrationSettingFrame::getModelIdx() const
{
    return ui->cbModel->currentIndex();
}

void IntergrationSettingFrame::setModels(const QStringList &modellist)
{
    ui->cbModel->clear();
    ui->cbModel->addItems(modellist);
}

void IntergrationSettingFrame::on_pushSolve_clicked()
{
    emit solveSig();
}

void IntergrationSettingFrame::on_cbModel_currentIndexChanged(int index)
{
    emit modelHasChanged(index);
}

void IntergrationSettingFrame::on_dsbStart_valueChanged(double value)
{
    ui->dsbEnd->setMinimum(value-0.1);
}
