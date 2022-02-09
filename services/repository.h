#ifndef SIPMREPOSITORY_H
#define SIPMREPOSITORY_H

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

    enum sipmQueryType {ALL_AFE, MAIN_AFE, EXT_AFE};

    void addDataToModelSipm(QStandardItemModel * model, QHash<QString, QVariant> * queryParameters) const;
    void addDataToModelScintillator(QStandardItemModel * model, QHash<QString, QVariant> * queryParameters) const;
    void addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName);

    QSqlQuery * getCountryComboBoxQuery() const
    {
        return countryComboBoxQuery;
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



private:
    QSqlDatabase * mcordDatabase;
    QSqlQuery * preparedAllAfeSipmQuery;
    QSqlQuery * preparedMainAfeSipmQuery;
    QSqlQuery * preparedExtAfeSipmQuery;
    QSqlQuery * preparedScintillatorQuery;
    QSqlQuery * countryComboBoxQuery;
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
    void addSipmQueryResultToModel(QStandardItemModel * model, QSqlQuery * query, const SipmQueryParameters * sipmParameters) const;
    void addScintillatorQueryResultToModel(QStandardItemModel * model, QSqlQuery * query, const ScintillatorQueryParameters * scintillatorParameters) const;
};

#endif // REPOZYTORY_H
