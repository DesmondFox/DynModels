#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include "../diffworker.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QWidget *parent = 0);
    ~ExportDialog();
    void setMethods(const QStringList &methodlist);
    void setData(const QList<ASolveByMethod> &solve, const QStringList &roles);
    void setCoefs(const QStringList &coefnames,
                  const QList<qreal> &values);
    void setPopulations(const QStringList &roles,
                       const QList<qreal> &values);
    void setPlots(const QPixmap &plot,
                  const QPixmap &phase);
    void setModel(const QString &modelName,
                  const QString &descr);

private slots:
    void on_buttons_accepted();

private:
    Ui::ExportDialog *ui;
    QList<ASolveByMethod> m_results;
    QStringList m_roles;
    void save(const QString &filename);
    QString getStartData();
    QString getTable();
    QString getEndData();
    QStringList m_coefs;
    QList<qreal> m_coefValues;
    QList<qreal> m_populValues;
    QPixmap m_phase;
    QPixmap m_plot;
    QString m_description;
    QString m_modelName;
};

#endif // EXPORTDIALOG_H
