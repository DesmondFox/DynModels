#ifndef LOGLOTKAVOLTERRA_H
#define LOGLOTKAVOLTERRA_H

#include "../idynmodelplugin.h"

class LogLotkaVolterra : public QObject,  IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "loglotkavolterra.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    explicit LogLotkaVolterra(QObject *parent = 0);
    virtual ~LogLotkaVolterra();

    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
};

#endif // LOGLOTKAVOLTERRA_H
