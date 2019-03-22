#include "verhulstmodel.h"


VerhulstModel::VerhulstModel(QObject *parent) :
    QGenericPlugin(parent)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(verhulst, VerhulstModel)
#endif // QT_VERSION < 0x050000
