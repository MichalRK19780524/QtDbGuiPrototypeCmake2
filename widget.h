#ifndef WIDGET_H
#define WIDGET_H

#include "wizard/addsipmwizard.h"
#include "services/repository.h"

#include <QWidget>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QStandardItemModel>
#include <QHash>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(Repository * repo, QWidget *parent = nullptr);
    ~Widget();
    const Repository * repository;
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

    void on_checkBox_afeMain_stateChanged(int arg1);

    void on_checkBox_afeExt_stateChanged(int arg1);

private:
    QHash<QString, QVariant> * getSipmQueryParameters() const;
    QHash<QString, QVariant> * getScintillatorQueryParameters() const;

    Ui::Widget * ui;

    QStringList * countries;
    QStringList * allAfeSerialNumberList;
    QStringList * mainAfeSerialNumberList;

    QStandardItemModel * mcordModelSipm;
    QStandardItemModel * mcordModelScintillator;
    QHash<QString, QVariant> * queryParameters;


    QCompleter * allAfeCompleter;

    QCompleter * mainAfeCompleter;

    QStringList * extAfeSerialNumberList;
    QCompleter * extAfeCompleter;

    AddSipmWizard *wizard;

};


#endif // WIDGET_H

