#ifndef LOTKAVOLTERRAMODEL_H
#define LOTKAVOLTERRAMODEL_H

#include <QGenericPlugin>


class LotkaVolterraModel : public QGenericPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QGenericPluginFactoryInterface" FILE "lotkavolterramodel.json")
#endif // QT_VERSION >= 0x050000

public:
    LotkaVolterraModel(QObject *parent = 0);
};

#endif // LOTKAVOLTERRAMODEL_H
