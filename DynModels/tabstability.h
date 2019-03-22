#ifndef TABSTABILITY_H
#define TABSTABILITY_H

#include <QWidget>
#include "../plugins/idynmodelplugin.h"
struct StablePointForPhasePortrait
{
    StablePointForPhasePortrait(Point pn, QString descr) {
        this->point = pn; this->description = descr;
    }
    Point point;
    QString description;
};


namespace Ui {
class TabStability;
}


class TabStability : public QWidget
{
    Q_OBJECT

public:
    explicit TabStability(QWidget *parent = nullptr);
    ~TabStability();
    void setVis(bool visible);
    void setPlugin(IDynModelPlugin *plugin);
    QList<StablePointForPhasePortrait> getEquilibriumPoints() const;

private:
    Ui::TabStability *ui;
    void findEquilibriumPoints(IDynModelPlugin *plugin);
    void findEigenpoints(IDynModelPlugin *plugin, QStringList pointList);
    QList<Point> equilPoints;
    QList<PointComplex> eigenPoints;
};

#endif // TABSTABILITY_H
