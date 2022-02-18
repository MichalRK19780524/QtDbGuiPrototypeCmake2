#ifndef SIPMREPOSITORY_H
#define SIPMREPOSITORY_H
#include "DTO/sipmdto.h"

#include <QObject>
#include <QtSql>
#include <QStandardItemModel>


class Repository: public QObject
{
    Q_OBJECT
public:
    Repository();
    ~Repository();
    static const QString allAfeSipmFilteredQueryString;
    static const QString mainAfeSipmFilteredQueryString;
    static const QString extAfeSipmFilteredQueryString;
    static const QString scintillatorFilteredQueryString;
    static const QString allAfeComboBoxQueryString;
    static const QString mainAfeComboBoxQueryString;
    static const QString extAfeComboBoxQueryString;
    static const QString countryComboBoxQueryString;
    static const QString institutionComboBoxQueryString;
    static const QString roomNoComboBoxQueryString;
    static const QString sipmModelComboBoxQueryString;
    static const QString deviceInsertingQueryString;
    static const QString sipmInsertingQueryString;

    enum sipmQueryType {ALL_AFE, MAIN_AFE, EXT_AFE};

    void addDataToModelSipm(QStandardItemModel * model, QHash<QString, QVariant> * queryParameters) const;
//    void insertDataFromSipmModel(QStandardItemModel * model) const;
    bool saveSipmData(const SipmDto * sipmData) const;
    void addDataToModelScintillator(QStandardItemModel * model, QHash<QString, QVariant> * queryParameters) const;
    void addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName) const;
    void addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName, QHash<QString, QVariant> parameters) const;


    QSqlQuery * getCountryComboBoxQuery() const
    {
        return countryComboBoxQuery;
    }

    QSqlQuery * getInstitutionComboBoxQuery() const
    {
        return institutionComboBoxQuery;
    }

    QSqlQuery * getRoomNoComboBoxQuery() const
    {
        return roomNoComboBoxQuery;
    }

    QStringList * getAllAfeSerialNumberList() const
    {
        return allAfeSerialNumberList;
    }

    QStringList * getMainAfeSerialNumberList() const
    {
        return mainAfeSerialNumberList;
    }

    QStringList * getExtAfeSerialNumberList() const
    {
        return extAfeSerialNumberList;
    }

    QStandardItemModel * getMcordModelSipm() const
    {
        return mcordModelSipm;
    }

    QSqlQuery * getSipmModelComboBoxQuery() const
    {
        return sipmModelComboBoxQuery;
    }

private:
    QSqlDatabase * mcordDatabase;
    QSqlQuery * preparedAllAfeSipmQuery;
    QSqlQuery * preparedMainAfeSipmQuery;
    QSqlQuery * preparedExtAfeSipmQuery;
    QSqlQuery * preparedDeviceInsertingQuery;
    QSqlQuery * preparedSipmInsertingQuery;
    QSqlQuery * preparedScintillatorQuery;
    QSqlQuery * countryComboBoxQuery;
    QSqlQuery * institutionComboBoxQuery;
    QSqlQuery * roomNoComboBoxQuery;
    QSqlQuery * sipmModelComboBoxQuery;
    QStandardItemModel * mcordModelSipm;
    QStandardItemModel * mcordModelScintillator;
    QStringList * countries;
    QStringList * allAfeSerialNumberList;
    QStringList * mainAfeSerialNumberList;
    QStringList * extAfeSerialNumberList;

    struct SipmQueryParameters
    {
        QVariant serialNumberOrNull;
        QVariant countryOrNull;
        QVariant statusOrNull;
        QVariant institutionOrNull;
        QVariant deviceModelOrNull;
        QVariant roomOrNull;
        QDateTime purchaseDateFrom;
        QDateTime purchaseDateTo;
        double vBrFrom;
        double vBrTo;
        double vOpFrom;
        double vOpTo;
        double darkCurrentFrom;
        double darkCurrentTo;
        QVariant afeSerialNumberOrNull;
    };

    struct ScintillatorQueryParameters
    {
        QVariant serialNumberOrNull;
        QVariant countryOrNull;
        QVariant statusOrNull;
        QVariant institutionOrNull;
        QVariant deviceModelOrNull;
        QVariant roomOrNull;
        QDateTime purchaseDateFrom;
        QDateTime purchaseDateTo;
    };

    QSqlQuery * createQuery(QSqlDatabase * mcordDatabase, QString queryString);
    void removeQuery(QSqlQuery * query);
    void openDatabase();
    void closeDatabase();
    void addSipmQueryResultToModel(QStandardItemModel * model, QSqlQuery * query, const SipmQueryParameters * sipmParameters) const;
    void addScintillatorQueryResultToModel(QStandardItemModel * model, QSqlQuery * query, const ScintillatorQueryParameters * scintillatorParameters) const;
};

#endif // REPOZYTORY_H
