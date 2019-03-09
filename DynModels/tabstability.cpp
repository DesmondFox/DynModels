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
    ui->lblEigenValues->setVisible(visible);
    ui->label_eigen->setVisible(visible);
    ui->label_eq->setVisible(visible);
}

void TabStability::findEquilibriumPoints(IDynModelPlugin *plugin)
{
    QStringList line;
    for (Point point : plugin->getEquilibriumPoints())
    {
        QString tmp = "(";
        QStringList vallist;
        for (qreal val : point)
            vallist << QString::number(val);
        tmp += vallist.join("; ");
        tmp += ")";
        line << tmp;
    }
    ui->lblEquil->setText(line.join(", "));
}

void TabStability::findEigenpoints(IDynModelPlugin *plugin)
{
    int eigen_idx = -1;
    auto values = plugin->getEquilibriumPoints();
    auto start = plugin->getStartValues();
    for (int i = 0; i < values.length(); i++)
    {
        bool equal = true;
        for (int j = 0; j < values.at(i).length(); j++)
            if (!qFuzzyCompare(values.at(i).at(j), start.at(j)))
            {
                equal = false;
                break;
            }
        if (equal)
        {
            eigen_idx = i;
            break;
        }
    }

    auto eigenvalues = plugin->getEigenvalues();
    if (eigen_idx == -1)
        ui->lblEigenValues->setText(plugin->getEigenvaluesSolve());
    else
    {
        QStringList line;
        for (qreal val : eigenvalues.at(eigen_idx))
            line << QString::number(val);
        ui->lblEigenValues->setText("["+line.join("; ")+"]");
    }
}

void TabStability::setPlugin(IDynModelPlugin *plugin)
{
    findEquilibriumPoints(plugin);
    findEigenpoints(plugin);
}
