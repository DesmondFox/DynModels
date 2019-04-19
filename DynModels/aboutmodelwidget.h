#ifndef ABOUTMODELWIDGET_H
#define ABOUTMODELWIDGET_H

#include <QTextEdit>
#include "../plugins/idynmodelplugin.h"

struct StablePointForPhasePortrait
{
    StablePointForPhasePortrait(Point pn, QString descr) {
        this->point = pn; this->description = descr;
    }
    Point point;
    QString description;
};


class AboutModelWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit AboutModelWidget(QWidget *parent = nullptr);
    void setStabDescription(IDynModelPlugin *plugin);
    QList<StablePointForPhasePortrait> getEquilibriumPoints() const;
    void setVis(bool visible);
    void setDescription(const QString &str);

private:
    void findEquilibriumPoints(IDynModelPlugin *plugin);
    void findEigenpoints(IDynModelPlugin *plugin, QStringList pointList);
    QList<StabilityPoint> equilPoints;
    QList<PointComplex> eigenPoints;

    QString stabilityStr;
    QString mainText;
};

#endif // ABOUTMODELWIDGET_H
