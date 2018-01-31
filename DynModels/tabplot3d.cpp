#include "tabplot3d.h"
#include "ui_tabplot3d.h"

TabPlot3D::TabPlot3D(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabPlot3D)
{
    ui->setupUi(this);
}

TabPlot3D::~TabPlot3D()
{
    delete ui;
}

void TabPlot3D::setData(const QList<ASolveByMethod> &solve, const QStringList &rolelist)
{
    results = solve;
    roles = rolelist;
}

void TabPlot3D::drawPlot()
{
    ui->plot3d->clear();
    ui->plot3d->setRoles(roles);
    if (ui->cbEulers3d->isChecked())
        ui->plot3d->draw(results.at(0).elements, Qt::blue);
    if (ui->cbModEulers3d->isChecked())
        ui->plot3d->draw(results.at(1).elements, Qt::red);
    if (ui->cbRungeKutta3d->isChecked())
        ui->plot3d->draw(results.at(2).elements, Qt::green);
    if (ui->cbAdams3d->isChecked())
        ui->plot3d->draw(results.at(3).elements, Qt::magenta);
}

void TabPlot3D::on_cbEulers3d_clicked()
{
    ui->plot3d->clear();
    ui->plot3d->draw(results.at(0).elements, Qt::blue);
}

void TabPlot3D::on_cbModEulers3d_clicked()
{
    ui->plot3d->clear();
    ui->plot3d->draw(results.at(1).elements, Qt::red);
}

void TabPlot3D::on_cbRungeKutta3d_clicked()
{
    ui->plot3d->clear();
    ui->plot3d->draw(results.at(2).elements, Qt::green);
}

void TabPlot3D::on_cbAdams3d_clicked()
{
    ui->plot3d->clear();
    ui->plot3d->draw(results.at(3).elements, Qt::magenta);
}
