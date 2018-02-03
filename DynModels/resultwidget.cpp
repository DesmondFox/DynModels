#include "resultwidget.h"
#include "ui_resultwidget.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);

    connect(ui->tab2DCurves,    SIGNAL(mouseHover(QPointF)), this, SIGNAL(sigMouseHoveredOn2DPlots(QPointF)));
    connect(ui->tabPhase,       SIGNAL(mouseHover(QPointF)), this, SIGNAL(sigMouseHoveredOn2DPlots(QPointF)));
    connect(ui->tabWidget,      SIGNAL(currentChanged(int)), this, SLOT(slotTabWidgetIndexChanged(int)));
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::setData(const QList<ASolveByMethod> &solve, const QStringList &roleslist)
{
    results.clear();
    results = solve;
    roles   = roleslist;
    ui->tab3DPlot->setData(solve, roleslist);
    ui->tabTable->setData(solve, roleslist);

    // Количество элементов
    quint8 size = solve.first().elements.first().second.size();
    if (size == 2)
        ui->tabPhase->setEnabled(true);
    if (size == 3)
        ui->tabPhase->setEnabled(false);


    if (ui->tabWidget->currentIndex() == 0)
        this->drawGraphs();
    if (ui->tabWidget->currentIndex() == 1)
        ui->tab3DPlot->drawPlot();
    if (ui->tabWidget->currentIndex() == 2)
        ui->tabTable->printTable();
}

void ResultWidget::on_cbEulers_clicked()
{
    checkClicked(ui->cbEulers, DiffMethod::Eilers, 0);
}

void ResultWidget::on_cbModEulers_clicked()
{
    checkClicked(ui->cbModEulers, DiffMethod::ModifiedEilers, 1);
}

void ResultWidget::on_cbRungeKutta_clicked()
{
    checkClicked(ui->cbRungeKutta, DiffMethod::RungeKutta4thOrder, 2);
}

void ResultWidget::on_cbAdams_clicked()
{
    checkClicked(ui->cbAdams, DiffMethod::AdamsBashforth4rdOrder, 3);
}

void ResultWidget::slotTabWidgetIndexChanged(int index)
{
    if (results.isEmpty())
        return;

    if (index == 0)
        this->drawGraphs();
    if (index == 1)
        ui->tab3DPlot->drawPlot();
    if (index == 2)
        ui->tabTable->printTable();
}

void ResultWidget::drawGraphs()
{
    Q_ASSERT(results.size() == DiffWorker::MethodNum);
    ui->tab2DCurves->clearPlot();
    if (!roles.isEmpty())
    {
        ui->tab2DCurves->setRoles(roles);
        if (results.first().elements.first().second.size() == 2)
            ui->tabPhase->setRoles(roles);
        ui->tab2DCurves->legend->setVisible(true);

    }
    bool hasChecked = false;
    if (ui->cbEulers->isChecked())
    {
        draw2DPlots(0, DiffMethod::Eilers);
        hasChecked = true;
    }
    if (ui->cbModEulers->isChecked())
    {
        draw2DPlots(1, DiffMethod::ModifiedEilers);
        hasChecked = true;
    }
    if (ui->cbRungeKutta->isChecked())
    {
        draw2DPlots(2, DiffMethod::RungeKutta4thOrder);
        hasChecked = true;
    }
    if (ui->cbAdams->isChecked())
    {
        draw2DPlots(3, DiffMethod::AdamsBashforth4rdOrder);
        hasChecked = true;
    }

    // Если нету отмеченых чекбоксов, то рисуем по методу Эйлера
    if (!hasChecked)
    {
        draw2DPlots(0, DiffMethod::Eilers);
        ui->cbEulers->setChecked(true);
    }
}

QPixmap ResultWidget::getPlotPix() const
{
    return ui->tab2DCurves->toPixmap();
}

QPixmap ResultWidget::getPhasePix() const
{
    return ui->tabPhase->toPixmap();
}

void ResultWidget::draw2DPlots(const quint8 &index, const DiffMethod &method)
{
    ui->tab2DCurves->draw(method, results.at(index).elements);
    if (getSizeOfData() == 2)
    {
        ui->tabPhase->draw(method, results.at(index).elements);
        ui->tabPhase->setEnabled(true);
    }
    else
        ui->tabPhase->setEnabled(false);
}

void ResultWidget::checkClicked(const QCheckBox *cb, const DiffMethod &method, const quint8 &index)
{
    if (results.isEmpty())
        return;
    if (cb->isChecked())
    {
        draw2DPlots(index, method);
    }
    else
    {
        ui->tab2DCurves->hide(method);
        ui->tabPhase->hide(method);
    }
}

quint8 ResultWidget::getSizeOfData() const
{
    if (results.isEmpty())
        return -1;
    return results.first().elements.first().second.size();
}
