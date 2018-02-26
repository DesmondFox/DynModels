#ifndef LOTKAVOLTERRAMODEL_H
#define LOTKAVOLTERRAMODEL_H

#include "../idynmodelplugin.h"

class LotkaVolterraModel : public QObject, public IDynModelPlugin
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID IDynModelPlugin_iid FILE "lotkavolterramodel.json")
    Q_INTERFACES(IDynModelPlugin)
#endif // QT_VERSION >= 0x050000

public:
    explicit LotkaVolterraModel(QObject *parent = 0);
    virtual ~LotkaVolterraModel();
    // IDynModelPlugin interface
public:
    QList<Element> differentiate(const DiffSettings &settings);
    QPixmap getFormulaPixmap();
};

#endif // LOTKAVOLTERRAMODEL_H
