#ifndef PLOT3D_H
#define PLOT3D_H

#include <QWidget>
#include <Q3DScatter>
#include "diffworker.h"

using namespace QtDataVisualization;
class Plot3D : public QWidget
{
    Q_OBJECT
public:
    explicit Plot3D(QWidget *parent = nullptr);
    void clear();
    void setRoles(const QStringList &roleslist);
    void draw(const QList<Element> &data, const Qt::GlobalColor &color);
    bool isLoaded() const;
private:
    Q3DScatter *p3dScatter;
    QScatter3DSeries *pSeries;
    QStringList roles;
    bool isCorrected(const qreal &val1,
                     const qreal &val2,
                     const qreal &val3);
};

#endif // PLOT3D_H
