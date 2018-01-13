#include "lotkavolterramodel.h"


LotkaVolterraModel::LotkaVolterraModel(QObject *parent) :
    QGenericPlugin(parent)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(lotkavolterramodel, LotkaVolterraModel)
#endif // QT_VERSION < 0x050000
