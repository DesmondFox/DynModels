#ifndef JACOBMONOD_H
#define JACOBMONOD_H

#include "../idynmodelplugin.h"

class JacobMonod : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "jacobmonod.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    explicit JacobMonod(QObject *parent = 0);
    virtual ~JacobMonod();
    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
};

#endif // JACOBMONOD_H
