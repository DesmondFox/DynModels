#include "tabtable.h"
#include "ui_tabtable.h"

TabTable::TabTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabTable),
    pModel(new TableDataModel(this))
{
    ui->setupUi(this);
    ui->tableView->setModel(pModel);
}

TabTable::~TabTable()
{
    delete ui;
}

void TabTable::setData(const QList<ASolveByMethod> &data, const QStringList &roleslist)
{
    roles = roleslist;
    results = data;
}

void TabTable::printTable()
{
    pModel->setRoles(roles);
    if (ui->rbEilers->isChecked())
        pModel->print(results.at(0).elements, roles);
    if (ui->rbModEilers->isChecked())
        pModel->print(results.at(1).elements, roles);
    if (ui->rbRungeKutta->isChecked())
        pModel->print(results.at(2).elements, roles);
    if (ui->rbAdams->isChecked())
        pModel->print(results.at(3).elements, roles);
}


void TabTable::on_rbEilers_clicked()
{
    if (ui->rbEilers->isChecked())
        pModel->print(results.at(0).elements, roles);
}

void TabTable::on_rbModEilers_clicked()
{
    if (ui->rbModEilers->isChecked())
        pModel->print(results.at(1).elements, roles);
}

void TabTable::on_rbRungeKutta_clicked()
{

    if (ui->rbRungeKutta->isChecked())
        pModel->print(results.at(2).elements, roles);
}

void TabTable::on_rbAdams_clicked()
{
    if (ui->rbAdams->isChecked())
        pModel->print(results.at(3).elements, roles);
}
