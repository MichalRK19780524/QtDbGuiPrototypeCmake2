#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QStandardItemModel>
#include "wizard/addsipmwizard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    static const QString sipmFilteredQueryString;
    static const QString scintillatorFilteredQueryString;
    static const QString afeComboBoxQueryString;
    static const QString countryComboBoxQueryString;
    static const QString institutionComboBoxQueryString;
    static const QString roomNoComboBoxQueryString;
    static constexpr double vBrFrom = 50.0;
    static constexpr double vBrTo = 65.0;
    static constexpr double vOpFrom = 50.0;
    static constexpr double vOpTo = 65.0;
    static constexpr double darkCurrentFrom = 0.5;
    static constexpr double darkCurrentTo = 0.9;

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_vBrReset_clicked();

    void on_pushButton_vOpReset_clicked();

    void on_pushButton_darkCurrentReset_clicked();

    void on_pushButton_searchScintillator_clicked();

    void on_pushButton_add_clicked();

private:
    Ui::Widget * ui;
    QSqlDatabase * mcordDatabase;
    QList<QSqlQuery *> * preparedQueries;
    QSqlQuery * preparedScintillatorQuery;
    QStandardItemModel * mcordModelSipm;
    QStandardItemModel * mcordModelScintillator;
    QStringList * countries;

    AddSipmWizard *wizard;

    void addDataToModelSipm(QStandardItemModel * model, QList<QSqlQuery *> * queries);
    void addDataToModelScintillator(QStandardItemModel * model, QSqlQuery * query);
    void addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName);
    static QList<QSqlQuery *> * createQueriesSipm(QSqlDatabase * mcordDatabase);
    QSqlQuery * createQuery(QSqlDatabase * mcordDatabase, QString queryString);
    void removeQuery(QSqlQuery * query);
};


#endif // WIDGET_H

