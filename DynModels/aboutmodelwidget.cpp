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
    stabilityStr += "<p><font size=\"5\">Точки рівноваги: ";
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
    if (plugin->getEigenvalues().size() == 0) {
        stabilityStr += "</font></p>";
        return;
    }

    stabilityStr += "Власні значення: ";
    equilPoints = plugin->getEquilibriumPoints();
    eigenPoints = plugin->getEigenvalues();

    QStringList lines;
    for (int i = 0; i < eigenPoints.size(); i++) {

        QStringList pn;
        PointComplex ePt = eigenPoints.at(i);
        for (int j = 0; j < ePt.size(); j++)
            pn.append(QString::number(ePt.at(j).lambda));


        QString line = QString("%1 - <b>%2</b>")
                .arg(pointList.at(i))
                .arg("["+pn.join("; ")+"]");
        if (!plugin->resolveLambdas(ePt).isEmpty())
            line += QString(" -> %1").arg(plugin->resolveLambdas(ePt));
        lines.append(line);
    }
    stabilityStr += lines.join("<br>");
    stabilityStr += "</font></p>";
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
