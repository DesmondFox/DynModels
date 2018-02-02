#include "populationsetframe.h"
#include "ui_populationsetframe.h"

PopulationSetFrame::PopulationSetFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PopulationSetFrame)
{
    ui->setupUi(this);
}

PopulationSetFrame::~PopulationSetFrame()
{
    delete ui;
}

void PopulationSetFrame::setFields(const QStringList &roles)
{
    Q_ASSERT(roles.size() > 1 && roles.size() <= 3);

    ui->labelField1->setText(roles.at(0));
    ui->labelField2->setText(roles.at(1));
    if (roles.size() == 3)
    {
        ui->labelField3->setText(roles.at(2));
        ui->labelField3->setHidden(false);
        ui->dsbField3->setHidden(false);
    }
    if (roles.size() == 2)
    {
        ui->labelField3->setHidden(true);
        ui->dsbField3->setHidden(true);
    }
}

QList<qreal> PopulationSetFrame::getValues() const
{
    QList<qreal> out;

    out << ui->dsbField1->value()
        << ui->dsbField2->value();
    if (!ui->dsbField3->isHidden())
        out << ui->dsbField3->value();

    return out;
}

void PopulationSetFrame::setValues(const QList<qreal> &values)
{
    ui->dsbField1->setValue(values.at(0));
    ui->dsbField2->setValue(values.at(1));
    if (values.size() == 3)
        ui->dsbField3->setValue(values.at(2));
}
