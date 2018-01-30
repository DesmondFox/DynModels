#ifndef COEFITEM_H
#define COEFITEM_H

#include <QWidget>

namespace Ui {
class CoefItem;
}

class CoefItem : public QWidget
{
    Q_OBJECT

public:
    explicit CoefItem(QWidget *parent = 0);
    ~CoefItem();

    qreal   getValue() const;
    void    setLabel(const QString &name);
    void    setValue(const qreal &value);
    void    setTip(const QString &tip);

private:
    Ui::CoefItem *ui;
};

#endif // COEFITEM_H
