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
        pItm->setSizeHint(pWgt->sizeHint());

        pWgt->setLabel(itm.coefName);
        pWgt->setValue(itm.value);
        pWgt->setTip(itm.tip);

        pItm->setBackgroundColor(QColor(200, 200, 200, 100));
        ui->listWidget->setItemWidget(pItm, pWgt);
        items << pWgt;
    }
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
