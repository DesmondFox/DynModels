#include "exportdialog.h"
#include "ui_exportdialog.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::setMethods(const QStringList &methodlist)
{
    ui->cbMethods->addItems(methodlist);
}

void ExportDialog::setData(const QList<ASolveByMethod> &solve,
                           const QStringList &roles)
{
    m_results   = solve;
    m_roles     = roles;
}

void ExportDialog::setCoefs(const QStringList &coefnames, const QList<qreal> &values)
{
    Q_ASSERT(coefnames.size() == values.size());
    m_coefs     = coefnames;
    m_coefValues= values;
}

void ExportDialog::setPopulations(const QStringList &roles, const QList<qreal> &values)
{
    Q_ASSERT(roles.size() == values.size());
    m_roles     = roles;
    m_populValues = values;
}

void ExportDialog::setPlots(const QPixmap &plot, const QPixmap &phase)
{
    m_plot  = plot;
    m_phase = phase;
}

void ExportDialog::setModel(const QString &modelName, const QString &descr)
{
    m_description   = descr;
    m_modelName     = modelName;
}

bool ExportDialog::hasData() const
{
    return !m_results.isEmpty();
}

void ExportDialog::on_buttons_accepted()
{
    bool textChecked = false;
    if (ui->cbStartData->isChecked())
        textChecked = true;
    if (ui->cbTable->isChecked())
        textChecked = true;
    if (ui->cbEndData->isChecked())
        textChecked = true;

    if (!textChecked && !ui->cbSavePhase->isChecked()
            && !ui->cbSavePlot->isChecked())
    {
        QMessageBox::critical(this, "Ошибка", "Сохранять нечего");
        return;
    }

    if (textChecked)
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение отчета"),
                                                        QDir::homePath(), tr("Text (*.txt)"));
        if (filename.isEmpty())
            return;
        save(filename);
    }

    if (ui->cbSavePlot->isChecked())
    {
        if (!m_phase.isNull())
        {
            QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение графика"),
                                                            QDir::homePath(), tr("PNG (*.png)"));
            if (filename.isEmpty())
                return;
            m_plot.save(filename);
        }
    }

    if (ui->cbSavePhase->isChecked())
    {
        if (!m_phase.isNull())
        {
            QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение фазового портрета"),
                                                            QDir::homePath(), tr("PNG (*.png)"));
            if (filename.isEmpty())
                return;
            m_phase.save(filename);
        }
    }
}

void ExportDialog::save(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Ошибка"), "Ошибка: "+file.errorString());
        return;
    }

    qreal startTime = m_results.first().elements.first().first;
    qreal endTime   = m_results.first().elements.back().first;
    qreal step      = m_results.first().elements.at(1).first -
            m_results.first().elements.at(0).first;

    QTextStream strm(&file);
    strm << tr("===== R E P O R T =====\n");
    strm << QString("Program:\t%1\n").arg(QApplication::applicationName());
    strm << QString("Version:\t%1\n").arg(QApplication::applicationVersion());
    strm << QString("Author:\t%1\n").arg(QString("Alexey Rott. 4IS17 2018"));
    strm << QString("Date: %1\n").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss::zzz"));
    strm << tr("===== B E G I N =====\n");
    strm << QString("Method:\t%1\n").arg(ui->cbMethods->currentText());
    strm << QString("Model:\t%1\n").arg(m_modelName);
    strm << QString("Model description:\t%1\n").arg(m_description);
    strm << QString("Interval: %1 - %2; Step: %3\n")
            .arg(startTime)
            .arg(endTime)
            .arg(step);

    if (ui->cbStartData->isChecked())
        strm << getStartData();
    if (ui->cbTable->isChecked())
        strm << getTable();
    if (ui->cbEndData->isChecked())
        strm << getEndData();

    strm << tr("===== E N D =====");
    file.close();
}

QString ExportDialog::getStartData()
{
    QString out;
    out += "===== STARTING DATA =====\n";
    out += "Populations:\n";
    for (quint8 i = 0; i < m_roles.size(); i++)
        out += QString("%1\t - %2\n")
               .arg(m_roles.at(i))
               .arg(m_populValues.at(i));
    out += "Coefficients:\n";
    for (quint8 i = 0; i < m_coefs.size(); i++)
        out += QString("%1\t - %2\n")
               .arg(m_coefs.at(i))
               .arg(m_coefValues.at(i));
    return out;
}

QString ExportDialog::getTable()
{
    QString out;
    out += "===== TABLE =====\n";
    out += "\tTime\t";
    for (quint8 i = 0; i < m_roles.size(); i++)
        out += m_roles.at(i)+"\t";
    out += "\n";

    auto res = m_results.at(ui->cbMethods->currentIndex()).elements;
    for (const Element &el : res)
    {
        qreal t = el.first;
        out += "\t"+QString::number(t)+"\t";
        for (quint8 r = 0; r < el.second.size(); r++)
            out += QString::number(el.second.at(r))+"\t";
        out += "\n";
    }
    return out;
}

QString ExportDialog::getEndData()
{
    QString out;
    out += "===== END DATA =====\n";
    Element end = m_results.at(ui->cbMethods->currentIndex()).elements.back();

    out += "At Time t = "+QString::number(end.first)+"\n";
    for (quint8 i = 0; i < m_roles.size(); i++)
        out += QString("%1\t = %2\n")
                .arg(m_roles.at(i))
                .arg(end.second.at(i));

    return out;
}
