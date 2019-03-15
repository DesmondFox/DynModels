#include "tabstability.h"
#include "ui_tabstability.h"
#include "utils/stabilityutil.h"

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
    findEigenpoints(plugin, line);
}

void TabStability::findEigenpoints(IDynModelPlugin *plugin, QStringList pointList)
{
    equilPoints = plugin->getEquilibriumPoints();
    eigenPoints = plugin->getEigenvalues();

    QStringList lines;
    for (int i = 0; i < equilPoints.size(); i++) {

        QStringList pn;
        Point eigenPoint = eigenPoints.at(i);
        for (int j = 0; j < eigenPoint.size(); j++)
            pn.append(QString::number(eigenPoint.at(j)));

        /// TODO: Добавить для 3 точек
        if (eigenPoint.size() == 2)
            lines.append(QString("%1 - <b>%2</b> ->&emsp;<font color=green>%3</font>")
                         .arg(pointList.at(i))
                         .arg("["+pn.join("; ")+"]")
                         .arg(StabilityUtil::resolveLambda(eigenPoint.at(0), eigenPoint.at(1))));


    }
    ui->lblEigenValues->setText(lines.join("<br>"));

}

QList<StablePointForPhasePortrait> TabStability::getEquilibriumPoints() const
{
    QList<StablePointForPhasePortrait> out;
    for (int i = 0; i < equilPoints.size(); i++) {
        /// TODO: Добавить для 3 точек
        if (equilPoints.at(i).size() == 2)
            out << StablePointForPhasePortrait(equilPoints[i],
                                               StabilityUtil::resolveLambda(
                                                    eigenPoints.at(i)[0],
                                                    eigenPoints.at(i)[1]));
    }
    return out;
}

void TabStability::setPlugin(IDynModelPlugin *plugin)
{
    findEquilibriumPoints(plugin);
}
