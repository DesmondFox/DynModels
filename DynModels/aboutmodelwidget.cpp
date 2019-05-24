#include "aboutmodelwidget.h"

AboutModelWidget::AboutModelWidget(QWidget *parent) : QTextEdit(parent)
{

}

void AboutModelWidget::setStabDescription(IDynModelPlugin *plugin)
{
    stabilityStr = "<hr>";
    findEquilibriumPoints(plugin);
    setHtml(mainText+stabilityStr);
}


void AboutModelWidget::setVis(bool visible)
{
}

void AboutModelWidget::setDescription(const QString &str)
{
    stabilityStr.clear();
    this->setHtml(str);
    mainText = str;
}

void AboutModelWidget::findEquilibriumPoints(IDynModelPlugin *plugin)
{
    stabilityStr += "Точки рівноваги: ";
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
    stabilityStr += line.join(", ");
    stabilityStr += "<br>";
    findEigenpoints(plugin, line);
}

void AboutModelWidget::findEigenpoints(IDynModelPlugin *plugin, QStringList pointList)
{
    stabilityStr += "Власні значення: ";
    equilPoints = plugin->getEquilibriumPoints();
    eigenPoints = plugin->getEigenvalues();

    QStringList lines;
    for (int i = 0; i < eigenPoints.size(); i++) {

        QStringList pn;
        PointComplex ePt = eigenPoints.at(i);
        for (int j = 0; j < ePt.size(); j++)
            pn.append(QString::number(ePt.at(j).lambda));

        /// TODO: Добавить для 3 точек
        lines.append(QString("%1 - <b>%2</b> -> <font color=green>%3</font>")
                     .arg(pointList.at(i))
                     .arg("["+pn.join("; ")+"]")
                     .arg(plugin->resolveLambdas(ePt)));


    }
    stabilityStr += lines.join("<br>");
}

QList<StablePointForPhasePortrait> AboutModelWidget::getEquilibriumPoints() const
{
    QList<StablePointForPhasePortrait> out;
    for (int i = 0; i < eigenPoints.size(); i++) {
        /// TODO: Добавить для 3 точек
        if (equilPoints.size() == 2)
            out << StablePointForPhasePortrait(equilPoints.at(i).point,
                                               equilPoints.at(i).formula);
    }
    return out;
}
