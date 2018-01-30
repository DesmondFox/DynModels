#include "coefitem.h"
#include "ui_coefitem.h"

CoefItem::CoefItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoefItem)
{
    ui->setupUi(this);
}

CoefItem::~CoefItem()
{
    delete ui;
}

qreal CoefItem::getValue() const
{
    return ui->dsbCoef->value();
}

void CoefItem::setLabel(const QString &name)
{
    ui->labelCoef->setText(name);
}

void CoefItem::setValue(const qreal &value)
{
    ui->dsbCoef->setValue(value);
}

void CoefItem::setTip(const QString &tip)
{
    this->setToolTip(tip);
}
