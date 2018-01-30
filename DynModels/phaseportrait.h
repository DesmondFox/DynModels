#ifndef PHASEPORTRAIT_H
#define PHASEPORTRAIT_H

#include <QWidget>
#include <qcustomplot.h>
#include "diffworker.h"

class PhasePortrait : public QCustomPlot
{
    Q_OBJECT
public:
    explicit PhasePortrait(QWidget *parent = nullptr);
    void hideSeries(const DiffMethod &method);
    void draw(const DiffMethod &method, const QList<Element> &data);
    void clearPlot();
    void setRoles(const QStringList &roleslist);

private:
    static const quint8 LineWidth   = 2;
    QCPGraph    *pEilersGraph;
    QCPGraph    *pModEilersGraph;
    QCPGraph    *pRKuttaGraph;
    QCPGraph    *pAdamsGraph;
    void prepareSeries();

    QStringList roles;
    QCPGraph    *getGraph(const DiffMethod &method);
    bool        isCorrected(const qreal &value);
};


#endif // PHASEPORTRAIT_H
