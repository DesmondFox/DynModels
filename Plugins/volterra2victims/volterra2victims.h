#ifndef VOLTERRA2VICTIMS_H
#define VOLTERRA2VICTIMS_H

#include "../idynmodelplugin.h"

class Volterra2Victims : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "volterra2victims.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    explicit Volterra2Victims(QObject *parent = 0);
    virtual ~Volterra2Victims();

    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
};

#endif // VOLTERRA2VICTIMS_H
