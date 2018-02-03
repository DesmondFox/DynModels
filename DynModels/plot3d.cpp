#include "plot3d.h"
#include <QHBoxLayout>

Plot3D::Plot3D(QWidget *parent)
    : QWidget(parent),
      p3dScatter(new Q3DScatter())
{
    QWidget *pContainer     = QWidget::createWindowContainer(p3dScatter);
    QHBoxLayout *pLayout    = new QHBoxLayout();
    pLayout->addWidget(pContainer);
    setLayout(pLayout);

    pSeries = new QScatter3DSeries(this);
    p3dScatter->addSeries(pSeries);
    pSeries->setMesh(QScatter3DSeries::MeshMinimal);
    p3dScatter->setShadowQuality(Q3DScatter::ShadowQualityLow);
    p3dScatter->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);
}

Plot3D::~Plot3D()
{
    p3dScatter->deleteLater();
}

void Plot3D::clear()
{
    pSeries->dataProxy()->removeItems(0, pSeries->dataProxy()->itemCount());
}

void Plot3D::setRoles(const QStringList &roleslist)
{
    if (roleslist.isEmpty())
        return;

    roles.clear();
    roles = roleslist;
    if (roles.size() == 2)
        roles << tr("Час, роки");
    p3dScatter->axisX()->setTitle(roles.at(0));
    p3dScatter->axisY()->setTitle(roles.at(1));
    p3dScatter->axisZ()->setTitle(roles.at(2));
    p3dScatter->axisX()->setTitleVisible(true);
    p3dScatter->axisY()->setTitleVisible(true);
    p3dScatter->axisZ()->setTitleVisible(true);
}

void Plot3D::draw(const QList<Element> &data, const Qt::GlobalColor &color)
{
    pSeries->setBaseColor(QColor(color));

    quint8 size = data.first().second.size();
    for (const Element &el : data)
    {
        qreal t = el.first;
        qreal x = el.second.at(0);
        qreal y = el.second.at(1);
        qreal z = 0.0;
        if (size == 3)
            z = el.second.at(2);

        if (!isCorrected(x, y, z))
        {
            qDebug() << "3D: incorrect data";
            break;
        }

        QScatterDataItem itm;
        itm.setX(x);
        itm.setY(y);
        if (size == 2)
            itm.setZ(t);
        if (size == 3)
            itm.setZ(z);
        pSeries->dataProxy()->addItem(itm);
    }
}

bool Plot3D::isCorrected(const qreal &val1, const qreal &val2, const qreal &val3)
{
    if (val1 > 100000)
        return false;
    if (val2 > 100000)
        return false;
    if (val3 > 100000)
        return false;
    return true;
}
