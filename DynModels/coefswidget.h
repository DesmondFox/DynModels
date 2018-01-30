#ifndef COEFSWIDGET_H
#define COEFSWIDGET_H

#include <QWidget>
#include "diffworker.h"

namespace Ui {
class CoefsWidget;
}

class CoefItem;

class CoefsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CoefsWidget(QWidget *parent = 0);
    ~CoefsWidget();
    void    setCoefs(const QList<Coef> &coefs);
    QList<qreal>    getValues() const;

private:
    Ui::CoefsWidget *ui;
    QList<CoefItem*> items;

    void    clear();
};

#endif // COEFSWIDGET_H
