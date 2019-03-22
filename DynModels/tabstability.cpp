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
    for (StabilityPoint point : plugin->getEquilibriumPoints())
    {
        QString tmp = "(";
        QStringList vallist;
        for (qreal val : point.point)
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
    equilPoints.clear();
    for (auto stabpoint : plugin->getEquilibriumPoints())
        equilPoints << stabpoint.point;
    eigenPoints = plugin->getEigenvalues();

    QStringList lines;
    for (int i = 0; i < equilPoints.size(); i++) {

        QStringList pn;
        PointComplex ePt = eigenPoints.at(i);
        for (int j = 0; j < ePt.size(); j++)
            pn.append(QString::number(ePt.at(j).lambda));

        /// TODO: Добавить для 3 точек
        if (ePt.size() == 2)
            lines.append(QString("%1 - <b>%2</b> -> <font color=green>%3</font>")
                         .arg(pointList.at(i))
                         .arg("["+pn.join("; ")+"]")
                         .arg(StabilityUtil::resolveLambda(ePt.at(0), ePt.at(1))));


    }
    ui->lblEigenValues->setText(lines.join("<br>"));

}

QList<StablePointForPhasePortrait> TabStability::getEquilibriumPoints() const
{
    QList<StablePointForPhasePortrait> out;
    for (int i = 0; i < eigenPoints.size(); i++) {
        /// TODO: Добавить для 3 точек
        if (equilPoints.size() == 2)
            out << StablePointForPhasePortrait(equilPoints.at(i),
                                               StabilityUtil::resolveLambda(eigenPoints[i].at(0),
                                                                            eigenPoints[i].at(1)));
    }
    return out;
}

void TabStability::setPlugin(IDynModelPlugin *plugin)
{
    findEquilibriumPoints(plugin);
}
