#ifndef VERHULSTMODEL_H
#define VERHULSTMODEL_H

#include <QGenericPlugin>


class VerhulstModel : public QGenericPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "verhulst.json")
#endif // QT_VERSION >= 0x050000

public:
    VerhulstModel(QObject *parent = 0);
};

#endif // VERHULSTMODEL_H
