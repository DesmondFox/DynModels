#include "resultdialog.h"
#include "ui_resultdialog.h"
#include <QGridLayout>

GBWidget::GBWidget(QWidget *parent)
    : QWidget(parent),
      pRole1(new QLabel(this)),
      pRole2(new QLabel(this)),
      pRole3(new QLabel(this)),
      pValue1(new QLabel(this)),
      pValue2(new QLabel(this)),
      pValue3(new QLabel(this))
{
    QGridLayout *pLay = new QGridLayout;
    pLay->addWidget(pRole1, 0, 0);
    pLay->addWidget(pRole2, 1, 0);
    pLay->addWidget(pRole3, 2, 0);
    pLay->addWidget(pValue1, 0, 1);
    pLay->addWidget(pValue2, 1, 1);
    pLay->addWidget(pValue3, 2, 1);
    pRole1->setText("dsds");
    setLayout(pLay);
}

void GBWidget::setValues(const QStringList &roles, const QVector<qreal> &values)
{
    if (roles.size() != values.size())
        return;

    quint8 size = roles.size();
    pRole1->setText(roles.at(0));
    pRole2->setText(roles.at(1));
    pValue1->setText(QString::number(static_cast<int>(values.at(0))));
    pValue2->setText(QString::number(static_cast<int>(values.at(1))));
    if (size == 3)
    {
        pRole3->setText(roles.at(2));
        pValue3->setText(QString::number(static_cast<int>(values.at(2))));
        pRole3->setVisible(true);
        pValue3->setVisible(true);
    }
    else
    {
        pRole3->setVisible(false);
        pValue3->setVisible(false);
    }
}


ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog),
    pEilers(new GBWidget(this)),
    pModEilers(new GBWidget(this)),
    pRKutta(new GBWidget(this)),
    pAdams(new GBWidget(this))
{
    ui->setupUi(this);
    QHBoxLayout *pHBox1 = new QHBoxLayout;
    pHBox1->addWidget(pEilers);
    ui->gbEilers->setLayout(pHBox1);

    QHBoxLayout *pHBox2 = new QHBoxLayout;
    pHBox2->addWidget(pModEilers);
    ui->gbModEilers->setLayout(pHBox2);

    QHBoxLayout *pHBox3 = new QHBoxLayout;
    pHBox3->addWidget(pRKutta);
    ui->gbRKutta->setLayout(pHBox3);

    QHBoxLayout *pHBox4 = new QHBoxLayout;
    pHBox4->addWidget(pAdams);
    ui->gbAdams->setLayout(pHBox4);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}

void ResultDialog::setValues(const QStringList &roles, const LastValues &values)
{
    pEilers->setValues(roles, values.eilers);
    pModEilers->setValues(roles, values.modEilers);
    pRKutta->setValues(roles, values.RKutta);
    pAdams->setValues(roles, values.adams);
}
