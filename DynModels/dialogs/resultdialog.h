#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>

namespace Ui {
class ResultDialog;
}

class QLabel;

class GBWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GBWidget(QWidget *parent = 0);
    void setValues(const QStringList &roles, const QVector<qreal> &values);

private:
    QLabel  *pRole1;
    QLabel  *pRole2;
    QLabel  *pRole3;
    QLabel  *pValue1;
    QLabel  *pValue2;
    QLabel  *pValue3;
};

struct LastValues
{
    QVector<qreal> eilers;
    QVector<qreal> modEilers;
    QVector<qreal> RKutta;
    QVector<qreal> adams;
};

class ResultDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResultDialog(QWidget *parent = 0);
    ~ResultDialog();
    void setValues(const QStringList &roles,
                   const LastValues  &values);
private:
    Ui::ResultDialog *ui;
    GBWidget    *pEilers;
    GBWidget    *pModEilers;
    GBWidget    *pRKutta;
    GBWidget    *pAdams;
};

#endif // RESULTDIALOG_H
