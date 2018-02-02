#ifndef TABTABLE_H
#define TABTABLE_H

#include <QWidget>
#include "tabledatamodel.h"

namespace Ui {
class TabTable;
}

class TabTable : public QWidget
{
    Q_OBJECT

public:
    explicit TabTable(QWidget *parent = 0);
    ~TabTable();
    void setData(const QList<ASolveByMethod> &data,
                 const QStringList &roleslist);
    void printTable();

private slots:
    void on_rbEilers_clicked();

    void on_rbModEilers_clicked();

    void on_rbRungeKutta_clicked();

    void on_rbAdams_clicked();

private:
    Ui::TabTable *ui;
    TableDataModel *pModel;
    QList<ASolveByMethod> results;
    QStringList roles;
};

#endif // TABTABLE_H
