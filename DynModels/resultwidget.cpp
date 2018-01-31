#include "resultwidget.h"
#include "ui_resultwidget.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
//    connect(ui->tabGraphs, SIGNAL(currentChanged(int)), )
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

    // Рисуем графики по методам, которые отмечены
    drawPlots();
}

void ResultWidget::on_cbEulers_clicked()
{
    if (results.isEmpty())
        return;
    if (ui->cbEulers->isChecked())
    {
        ui->tab2DCurves->drawSeries(DiffMethod::Eilers, results.at(0).elements);
        /// TODO: Add phase portrait
    }
    else
        ui->tab2DCurves->hideSeries(DiffMethod::Eilers);
}

void ResultWidget::on_cbModEulers_clicked()
{
    if (results.isEmpty())
        return;
    if (ui->cbModEulers->isChecked())
    {
        ui->tab2DCurves->drawSeries(DiffMethod::ModifiedEilers, results.at(1).elements);
        /// TODO: Add phase portrait
    }
    else
        ui->tab2DCurves->hideSeries(DiffMethod::ModifiedEilers);
}

void ResultWidget::on_cbRungeKutta_clicked()
{
    if (results.isEmpty())
        return;
    if (ui->cbRungeKutta->isChecked())
    {
        ui->tab2DCurves->drawSeries(DiffMethod::RungeKutta4thOrder, results.at(2).elements);
        /// TODO: Add phase portrait
    }
    else
        ui->tab2DCurves->hideSeries(DiffMethod::RungeKutta4thOrder);
}

void ResultWidget::on_cbAdams_clicked()
{
    if (results.isEmpty())
        return;
    if (ui->cbAdams->isChecked())
    {
        ui->tab2DCurves->drawSeries(DiffMethod::AdamsBashforth4rdOrder, results.at(3).elements);
        /// TODO: Add phase portrait
    }
    else
        ui->tab2DCurves->hideSeries(DiffMethod::AdamsBashforth4rdOrder);
}

void ResultWidget::drawPlots()
{
    Q_ASSERT(results.size() == DiffWorker::MethodNum);

    ui->tabPhase->setRoles(roles);
    ui->tabPhase->draw(Eilers, results.at(0).elements);
    ui->tab2DCurves->clearPlot();
    if (!roles.isEmpty())
    {
        ui->tab2DCurves->setRoles(roles);
        ui->tab2DCurves->legend->setVisible(true);
    }
    bool hasChecked = false;
    if (ui->cbEulers->isChecked())
    {
        /// TODO: add phase portrait
        ui->tab2DCurves->drawSeries(DiffMethod::Eilers, results.at(0).elements);
        hasChecked = true;
    }
    if (ui->cbModEulers->isChecked())
    {
        /// TODO: add phase portrait
        ui->tab2DCurves->drawSeries(DiffMethod::ModifiedEilers, results.at(1).elements);
        hasChecked = true;
    }
    if (ui->cbRungeKutta->isChecked())
    {
        /// TODO: add phase portrait
        ui->tab2DCurves->drawSeries(DiffMethod::RungeKutta4thOrder, results.at(2).elements);
        hasChecked = true;
    }
    if (ui->cbAdams->isChecked())
    {
        /// TODO: add phase portrait
        ui->tab2DCurves->drawSeries(DiffMethod::AdamsBashforth4rdOrder, results.at(3).elements);
        hasChecked = true;
    }

    // Если нету отмеченых чекбоксов, то рисуем по методу Эйлера
    if (!hasChecked)
    {
        ui->cbEulers->setChecked(true);
        ui->tab2DCurves->drawSeries(DiffMethod::Eilers, results.at(0).elements);
    }
}
