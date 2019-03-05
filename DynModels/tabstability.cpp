#include "tabstability.h"
#include "ui_tabstability.h"

TabStability::TabStability(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabStability)
{
    ui->setupUi(this);
}

TabStability::~TabStability()
{
    delete ui;
}

void TabStability::setVis(bool visible)
{

    ui->noResultLbl->setVisible(!visible);
    ui->lblEquil->setVisible(visible);
}

void TabStability::findEquilibriumPoints(IDynModelPlugin *plugin)
{
    QString str = ui->lblEquil->text();
    QStringList dotList;

    for (QPair<qreal, qreal> point : plugin->getEquilibriumPoints())
        dotList << QString("(%1, %2)")
                   .arg(point.first)
                   .arg(point.second);
    ui->lblEquil->setText(QString(str).arg(dotList.join(", ")));
}

void TabStability::setPlugin(IDynModelPlugin *plugin)
{
    findEquilibriumPoints(plugin);
}
