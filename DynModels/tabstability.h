#ifndef TABSTABILITY_H
#define TABSTABILITY_H

#include <QWidget>
#include "../plugins/idynmodelplugin.h"
namespace Ui {
class TabStability;
}

class TabStability : public QWidget
{
    Q_OBJECT

public:
    explicit TabStability(QWidget *parent = nullptr);
    ~TabStability();
    void setVis(bool visible);
    void setPlugin(IDynModelPlugin *plugin);

private:
    Ui::TabStability *ui;
    void findEquilibriumPoints(IDynModelPlugin *plugin);
    void findEigenpoints(IDynModelPlugin *plugin);
};

#endif // TABSTABILITY_H
