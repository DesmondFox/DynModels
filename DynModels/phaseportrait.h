#ifndef PHASEPORTRAIT_H
#define PHASEPORTRAIT_H

#include <QWidget>
#include "commonplot.h"
#include "tabstability.h"

class PhasePortrait : public CommonPlot
{
    Q_OBJECT
public:
    explicit PhasePortrait(QWidget *parent = nullptr);
    virtual void draw(const DiffMethod &method, const QList<Element> &data);
    virtual void setRoles(const QStringList &roleslist);
    virtual void hide(const DiffMethod &method);
    virtual void clearPlot();
    void setEquilibriumPoints(QList<StablePointForPhasePortrait> points);

private:
    static const quint8 LineWidth   = 2;
    QCPCurve *pEilersCurve;
    QCPCurve *pModEilersCurve;
    QCPCurve *pRungeKuttaCurve;
    QCPCurve *pAdamsCurve;
    QCPGraph *pointsGraph;
    void prepareItems();
    QCPCurve *getCurve(const DiffMethod &method);
    QStringList roles;
    QList<StablePointForPhasePortrait> equilPoints;
    void drawEquilibriumPoints();
    QList<QCPItemText*> textItems;
};


#endif // PHASEPORTRAIT_H
