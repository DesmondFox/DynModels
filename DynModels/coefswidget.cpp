#include "coefswidget.h"
#include "ui_coefswidget.h"
#include "coefitem.h"
#include <QVBoxLayout>

CoefsWidget::CoefsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoefsWidget)
{
    ui->setupUi(this);
}

CoefsWidget::~CoefsWidget()
{
    delete ui;
}

void CoefsWidget::setCoefs(const QList<Coef> &coefs)
{
    clear();

    CoefItem *pWgt = nullptr;
    QListWidgetItem *pItm = nullptr;
    for (const Coef &itm : coefs)
    {
        pWgt = new CoefItem();
        pItm = new QListWidgetItem(ui->listWidget);

        QSize sizeHint = pWgt->sizeHint();
//        sizeHint.setWidth(sizeHint.width() + 20);

        pItm->setSizeHint(sizeHint);

        // Обьединенное название и буквой типа
        // Название (буква)
        QString concatenatedName = itm.coefName+" ("+itm.letter+")";
        pWgt->setLabel(concatenatedName);
        pWgt->setValue(itm.value);
        pWgt->setTip(itm.tip);

        pItm->setBackgroundColor(QColor(200, 200, 200, 100));
        ui->listWidget->setItemWidget(pItm, pWgt);
        items << pWgt;
    }
}

void CoefsWidget::setValues(const QList<qreal> &values)
{
    for (int i = 0; i < items.size(); i++)
        items.at(i)->setValue(values.at(i));
}

QList<qreal> CoefsWidget::getValues() const
{
    QList<qreal> out;
    for (const CoefItem *pWgt : items)
        out << pWgt->getValue();
    return out;
}

void CoefsWidget::clear()
{
    ui->listWidget->clear();
    items.clear();
}
