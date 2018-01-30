#ifndef HOLLINGTANNER_H
#define HOLLINGTANNER_H

#include "../idynmodelplugin.h"

class HollingTanner : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "holingtanner.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:

    explicit HollingTanner(QObject *parent = 0);
    virtual ~HollingTanner();

    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
};

#endif // HOLLINGTANNER_H
