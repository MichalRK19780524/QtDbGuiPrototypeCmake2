#include "repository.h"
#include "DTO/sipmmodeldto.h"
#include <QStandardItemModel>
#include <QLocale>
#include <stdexcept>


const QString Repository::allAfeSipmFilteredQueryString =
R"(
SELECT dsv.serial_number  AS serial_number,
       dsv.purchase_date  AS purchase_date,
       dsv.model 		  AS model,
       dsv.status         AS status,
       dsv.comment 	      AS comment,
       i.name             AS institution,
       c.name          	  AS country,
       r.name             AS room,
       dsv.v_br           AS v_br,
       dsv.v_op           AS v_op,
       dsv.dark_current   AS dark_current,
       dsv.id 		      AS id,
       CASE
            WHEN damv.serial_number ISNULL AND daev.serial_number ISNULL THEN 'NOT ESTABLISHED'
            WHEN damv.serial_number IS NOT NULL THEN 'MAIN'
            WHEN daev.serial_number IS NOT NULL THEN 'EXT'
       END
                          AS afe_type,
       coalesce(damv.serial_number, daev.serial_number) AS afe_serial_number
FROM location l
LEFT JOIN country c ON l.country_id = c.id
LEFT JOIN institution i ON l.institution_id = i.id
LEFT JOIN room r ON l.room_id = r.id
JOIN device_sipm_view dsv ON dsv.location_id = l.id
LEFT JOIN device_afe_main_view damv ON dsv.id = damv.sipm_id AND dsv.date_from = damv.sipm_date_from
LEFT JOIN device_afe_ext_view daev ON dsv.id = daev.sipm_id AND dsv.date_from = daev.sipm_date_from
WHERE (dsv.serial_number = coalesce(:serialNumber, dsv.serial_number) OR dsv.serial_number ISNULL)
    AND (c.name = coalesce(:country, c.name) OR c.name ISNULL)
    AND (i.name = coalesce(:institution, i.name) OR i.name ISNULL)
    AND (r.name = coalesce(:room, r.name) OR r.name  ISNULL)
    AND (dsv.purchase_date BETWEEN :purchaseDateFrom AND :purchaseDateTo OR dsv.purchase_date ISNULL)
    AND (dsv.status = coalesce(:status, dsv.status) OR dsv.status ISNULL)
    AND (dsv.model = coalesce(:model, dsv.model) OR dsv.model ISNULL)
    AND (dsv.v_br BETWEEN :vBrFrom AND :vBrTo OR dsv.v_br ISNULL)
    AND (dsv.v_op BETWEEN :vOpFrom AND :vOpTo OR dsv.v_op ISNULL)
    AND (dsv.dark_current BETWEEN :darkCurrentFrom AND :darkCurrentTo OR dsv.dark_current ISNULL)
    AND
    (
        CASE
            WHEN :afeSerialNumber ISNULL THEN true
            WHEN damv.serial_number ISNULL THEN false
            WHEN damv.serial_number = :afeSerialNumber THEN true
        END
        OR
        CASE
            WHEN daev.serial_number ISNULL THEN false
            WHEN daev.serial_number = :afeSerialNumber THEN true
        END
    ))";

const QString Repository::mainAfeSipmFilteredQueryString =
R"(
SELECT dsv.serial_number  AS serial_number,
       dsv.purchase_date  AS purchase_date,
       dsv.model 		  AS model,
       dsv.status         AS status,
       dsv.comment 	      AS comment,
       i.name             AS institution,
       c.name          	  AS country,
       r.name             AS room,
       dsv.v_br           AS v_br,
       dsv.v_op           AS v_op,
       dsv.dark_current   AS dark_current,
       dsv.id 		      AS id,
       CASE
            WHEN damv.serial_number ISNULL THEN 'NOT ESTABLISHED'
            WHEN damv.serial_number IS NOT NULL THEN 'MAIN'
       END
                          AS afe_type,
       damv.serial_number AS afe_serial_number
FROM location l
LEFT JOIN country c ON l.country_id = c.id
LEFT JOIN institution i ON l.institution_id = i.id
LEFT JOIN room r ON l.room_id = r.id
JOIN device_sipm_view dsv ON dsv.location_id = l.id
JOIN device_afe_main_view damv ON dsv.id = damv.sipm_id AND dsv.date_from = damv.sipm_date_from
WHERE (dsv.serial_number = coalesce(:serialNumber, dsv.serial_number) OR dsv.serial_number ISNULL)
    AND (c.name = coalesce(:country, c.name) OR c.name ISNULL)
    AND (i.name = coalesce(:institution, i.name) OR i.name ISNULL)
    AND (r.name = coalesce(:room, r.name) OR r.name  ISNULL)
    AND (dsv.purchase_date BETWEEN :purchaseDateFrom AND :purchaseDateTo OR dsv.purchase_date ISNULL)
    AND (dsv.status = coalesce(:status, dsv.status) OR dsv.status ISNULL)
    AND (dsv.model = coalesce(:model, dsv.model) OR dsv.model ISNULL)
    AND (dsv.v_br BETWEEN :vBrFrom AND :vBrTo OR dsv.v_br ISNULL)
    AND (dsv.v_op BETWEEN :vOpFrom AND :vOpTo OR dsv.v_op ISNULL)
    AND (dsv.dark_current BETWEEN :darkCurrentFrom AND :darkCurrentTo OR dsv.dark_current ISNULL)
    AND (damv.serial_number = coalesce(:afeSerialNumber, damv.serial_number)))";

const QString Repository::extAfeSipmFilteredQueryString =
R"(
SELECT dsv.serial_number  AS serial_number,
       dsv.purchase_date  AS purchase_date,
       dsv.model 		  AS model,
       dsv.status         AS status,
       dsv.comment 	      AS comment,
       i.name             AS institution,
       c.name          	  AS country,
       r.name             AS room,
       dsv.v_br           AS v_br,
       dsv.v_op           AS v_op,
       dsv.dark_current   AS dark_current,
       dsv.id 		      AS id,
       CASE
           WHEN daev.serial_number ISNULL THEN 'NOT ESTABLISHED'
           WHEN daev.serial_number IS NOT NULL THEN 'EXT'
       END
                      AS afe_type,
       daev.serial_number AS afe_serial_number
       FROM location l
       LEFT JOIN country c ON l.country_id = c.id
       LEFT JOIN institution i ON l.institution_id = i.id
       LEFT JOIN room r ON l.room_id = r.id
       JOIN device_sipm_view dsv ON dsv.location_id = l.id
       JOIN device_afe_ext_view daev ON dsv.id = daev.sipm_id AND dsv.date_from = daev.sipm_date_from
       WHERE (dsv.serial_number = coalesce(:serialNumber, dsv.serial_number) OR dsv.serial_number ISNULL)
            AND (c.name = coalesce(:country, c.name) OR c.name ISNULL)
            AND (i.name = coalesce(:institution, i.name) OR i.name ISNULL)
            AND (r.name = coalesce(:room, r.name) OR r.name  ISNULL)
            AND (dsv.purchase_date BETWEEN :purchaseDateFrom AND :purchaseDateTo OR dsv.purchase_date ISNULL)
            AND (dsv.status = coalesce(:status, dsv.status) OR dsv.status ISNULL)
            AND (dsv.model = coalesce(:model, dsv.model) OR dsv.model ISNULL)
            AND (dsv.v_br BETWEEN :vBrFrom AND :vBrTo OR dsv.v_br ISNULL)
            AND (dsv.v_op BETWEEN :vOpFrom AND :vOpTo OR dsv.v_op ISNULL)
            AND (dsv.dark_current BETWEEN :darkCurrentFrom AND :darkCurrentTo OR dsv.dark_current ISNULL)
            AND (daev.serial_number = coalesce(:afeSerialNumber, daev.serial_number)))";

const QString Repository::deviceInsertingQueryString =
R"(
INSERT INTO
        device (id,
        date_from,
        serial_number,
        purchase_date,
        status,
        comments,
        location_id)
        SELECT
            (
            SELECT
                MAX(id) + 1
            FROM
                device
            ),
            CAST((julianday('now') - 2440587.5)* 86400000 AS INTEGER),
            :serial_number,
            :purchase_date,
            'new',
            :comments,
            (
            SELECT
                id
            FROM
                country_institution_room_location_view
            WHERE
                country = :country
                AND institution = :institution
                AND room = :room
            )
            RETURNING id, date_from
)";

const QString Repository::sipmInsertingQueryString =
R"(
INSERT INTO
    sipm(id,
    date_from,
    v_br,
    v_op,
    dark_current,
    sipm_model_id)
SELECT
    :id,
    :date_from,
    :v_br,
    :v_op,
    :dark_current,
    (
        SELECT id
        FROM sipm_model
        WHERE name = :model_name
    )
)";

const QString Repository::sipmModelInsertingQueryString =
R"(
INSERT INTO
    sipm_model (name, overvoltage)
VALUES (:name, :overvoltage)
)";

const QString Repository::allAfeComboBoxQueryString =
R"(
SELECT serial_number
FROM device_afe_main_view
UNION
SELECT serial_number
FROM device_afe_ext_view daev)";

const QString Repository::mainAfeComboBoxQueryString =
R"(
SELECT serial_number
FROM device_afe_main_view)";

const QString Repository::extAfeComboBoxQueryString =
R"(
SELECT serial_number
FROM device_afe_ext_view daev)";

const QString Repository::countryComboBoxQueryString =
R"(
SELECT name
FROM country)";

const QString Repository::sipmModelComboBoxQueryString =
R"(
SELECT name
FROM sipm_model)";

const QString Repository::scintillatorModelComboBoxQueryString =
R"(
SELECT name
FROM scintillator_model)";

const QString Repository::institutionComboBoxQueryString =
R"(
SELECT i.name AS name
FROM institution i
JOIN country c ON i.country_id = c.id
WHERE c.name = coalesce(:country, c.name))";

const QString Repository::roomNoComboBoxQueryString =
R"(
SELECT r.name AS name
FROM room r
JOIN institution i ON r.institution_id = i.id
JOIN country c ON i.country_id = c.id
WHERE c.name = coalesce(:country, c.name)
AND   i.name = coalesce(:institution, i.name))";

const QString Repository::scintillatorFilteredQueryString =
R"(SELECT dsv.serial_number 	AS serial_number,
       dsv.purchase_date 	AS purchase_date,
       dsv.model 		   	AS model,
       dsv.status        	AS status,
       dsv.comment 	   		AS comment,
       i.name             	AS institution,
       c.name          	  	AS country,
       r.name             	AS room
FROM location l
LEFT JOIN country c ON l.country_id = c.id
LEFT JOIN institution i ON l.institution_id = i.id
LEFT JOIN room r ON l.room_id = r.id
JOIN device_scintillator_view dsv ON dsv.location_id = l.id
WHERE (dsv.serial_number = coalesce(:serialNumber, dsv.serial_number) OR dsv.serial_number ISNULL)
    AND (dsv.purchase_date BETWEEN :purchaseDateFrom AND :purchaseDateTo OR dsv.purchase_date ISNULL)
    AND (dsv.status = coalesce(:status, dsv.status) OR dsv.status ISNULL)
    AND (dsv.model = coalesce(:model, dsv.model) OR dsv.model ISNULL)
    AND (c.name = coalesce(:country, c.name) OR c.name ISNULL)
    AND (i.name = coalesce(:institution, i.name) OR i.name ISNULL)
    AND (r.name = coalesce(:room, r.name) OR r.name  ISNULL))";

Repository::Repository()
{
    openDatabase();
    mcordModelSipm = new QStandardItemModel(this);
    mcordModelScintillator = new QStandardItemModel(this);

    countryComboBoxQuery = createQuery(mcordDatabase, countryComboBoxQueryString);
    institutionComboBoxQuery = createQuery(mcordDatabase, institutionComboBoxQueryString);
    roomNoComboBoxQuery = createQuery(mcordDatabase, roomNoComboBoxQueryString);

    sipmModelComboBoxQuery = createQuery(mcordDatabase, sipmModelComboBoxQueryString);
    scintillatorModelComboBoxQuery = createQuery(mcordDatabase, scintillatorModelComboBoxQueryString);

    QSqlQuery * allAfeComboBoxQuery = createQuery(mcordDatabase, allAfeComboBoxQueryString);
    allAfeSerialNumberList = new QStringList();
    allAfeSerialNumberList->append("");
    addDataToStringList(allAfeSerialNumberList, allAfeComboBoxQuery, "serial_number");
    removeQuery(allAfeComboBoxQuery);

    QSqlQuery * mainAfeComboBoxQuery = createQuery(mcordDatabase, mainAfeComboBoxQueryString);
    mainAfeSerialNumberList = new QStringList();
    mainAfeSerialNumberList->append("");
    addDataToStringList(mainAfeSerialNumberList, mainAfeComboBoxQuery, "serial_number");
    removeQuery(mainAfeComboBoxQuery);

    QSqlQuery * extAfeComboBoxQuery = createQuery(mcordDatabase, extAfeComboBoxQueryString);
    extAfeSerialNumberList = new QStringList();
    extAfeSerialNumberList->append("");
    addDataToStringList(extAfeSerialNumberList, extAfeComboBoxQuery, "serial_number");
    removeQuery(extAfeComboBoxQuery);

    preparedAllAfeSipmQuery = createQuery(mcordDatabase, allAfeSipmFilteredQueryString);
    preparedMainAfeSipmQuery = createQuery(mcordDatabase, mainAfeSipmFilteredQueryString);
    preparedExtAfeSipmQuery = createQuery(mcordDatabase, extAfeSipmFilteredQueryString);
    preparedDeviceInsertingQuery = createQuery(mcordDatabase, deviceInsertingQueryString);
    preparedSipmInsertingQuery = createQuery(mcordDatabase, sipmInsertingQueryString);
    preparedScintillatorQuery = createQuery(mcordDatabase, scintillatorFilteredQueryString);
    preparedSipmModelInsertingQuery = createQuery(mcordDatabase, sipmModelInsertingQueryString);
}

Repository::~Repository()
{
    removeQuery(countryComboBoxQuery);
    removeQuery(institutionComboBoxQuery);
    removeQuery(preparedAllAfeSipmQuery);
    removeQuery(preparedMainAfeSipmQuery);
    removeQuery(preparedExtAfeSipmQuery);
    removeQuery(preparedDeviceInsertingQuery);
    removeQuery(preparedSipmInsertingQuery);
    removeQuery(preparedScintillatorQuery);
    removeQuery(preparedSipmModelInsertingQuery);
    delete mcordModelSipm;
    mcordModelSipm = nullptr;
    delete mcordModelScintillator;
    mcordModelScintillator = nullptr;
    delete allAfeSerialNumberList;
    allAfeSerialNumberList = nullptr;
    delete mainAfeSerialNumberList;
    mainAfeSerialNumberList = nullptr;
    delete extAfeSerialNumberList;
    extAfeSerialNumberList = nullptr;
    closeDatabase();
}

void Repository::openDatabase()
{
    mcordDatabase = new QSqlDatabase;
    *mcordDatabase = QSqlDatabase::addDatabase("QSQLITE");
    //TODO Przeniesc scieżkę z nazwa bazy danych do stałej w jakies sensowne miejsce
    //Docelowo to powinna być względna ścieżka do pliku (poczytać jak i gdzie Qt umieszcza standardowe zasoby), taka, żeby pod Linuksem też działało
    mcordDatabase->setDatabaseName("C:/Users/user/Documents/QtProjects/Qt_database_GUI_prototype/db/MCORD_Server_sqlite_db_test2.sqlite3");
    if(mcordDatabase->open())
    {
        qDebug() << "MCORD database has been opened";
    }
    else
    {
        qDebug() << "Error! MCORD database could not be opened";
    }
}

void Repository::closeDatabase(){
    mcordDatabase->close();
    delete mcordDatabase;
    mcordDatabase = nullptr;
}

void Repository::addSipmQueryResultToModel(QStandardItemModel * model, QSqlQuery * query, const SipmQueryParameters * sipmParameters) const
{
    query->bindValue(":serialNumber", sipmParameters->serialNumberOrNull);
    query->bindValue(":purchaseDateFrom", QVariant(sipmParameters->purchaseDateFrom.toMSecsSinceEpoch()));
    query->bindValue(":purchaseDateTo", QVariant(sipmParameters->purchaseDateTo.toMSecsSinceEpoch()));
    query->bindValue(":country", sipmParameters->countryOrNull);
    query->bindValue(":status", sipmParameters->statusOrNull);
    query->bindValue(":institution", sipmParameters->institutionOrNull);
    query->bindValue(":model", sipmParameters->sipmModelOrNull);
    query->bindValue(":room", sipmParameters->roomOrNull);
    query->bindValue(":vBrFrom", sipmParameters->vBrFrom);
    query->bindValue(":vBrTo", sipmParameters->vBrTo);
    query->bindValue(":vOpFrom", sipmParameters->vOpFrom);
    query->bindValue(":vOpTo", sipmParameters->vOpTo);
    query->bindValue(":darkCurrentFrom", sipmParameters->darkCurrentFrom);
    query->bindValue(":darkCurrentTo", sipmParameters->darkCurrentTo);
    query->bindValue(":afeSerialNumber", sipmParameters->afeSerialNumberOrNull);

    if(query->exec())
    {
        while(query->next())
        {
            long long purchaseDateMsc = query->value("purchase_date").toLongLong();
            QString purchaseDateText = "";
            if(purchaseDateMsc != 0)
            {
                purchaseDateText = QDateTime::fromMSecsSinceEpoch(purchaseDateMsc).toString("dd.MM.yyyy");
            }

            QList<QStandardItem*> rowList = {
                new QStandardItem("SiPM"),
                new QStandardItem(query->value("serial_number").toString()),
                new QStandardItem(purchaseDateText),
                new QStandardItem(query->value("model").toString()),
                new QStandardItem(query->value("status").toString()),
                new QStandardItem(query->value("institution").toString()),
                new QStandardItem(query->value("comment").toString()),
                new QStandardItem(query->value("country").toString()),
                new QStandardItem(query->value("room").toString()),
                new QStandardItem(query->value("v_br").isNull() ? "" : QString::number(query->value("v_br").toDouble(), 'f', 2)),
                new QStandardItem(query->value("v_op").isNull() ? "" : QString::number(query->value("v_op").toDouble(), 'f', 2)),
                new QStandardItem(query->value("dark_current").isNull() ? "" : QString::number(query->value("dark_current").toDouble(), 'f', 2)),
                new QStandardItem(query->value("afe_type").toString()),
                new QStandardItem(query->value("afe_serial_number").toString())
            };
            model->appendRow(rowList);
        }
    }
    else
    {
        qDebug() << "Error! The SiPM query has failed  " << query->lastError() << query->boundValues();
    }
}

void Repository::addDataToModelSipm(QStandardItemModel * model, QHash<QString, QVariant> * queryParameters) const
{
    model->clear();

    //TODO Nazwy kolumn przenieść do stałej i utworzyć jakieś miejsce do przechowywania tego typu informacji.
    // Docelowo wdrożyć narzędzie umożliwiające wielojęzyczność Qt Linguist
    QStringList headers;
    headers.append("Type");
    headers.append("Serial number");
    headers.append("Purchase date");
    headers.append("Model");
    headers.append("Status");
    headers.append("Institution");
    headers.append("Comment");
    headers.append("Country");
    headers.append("Room");
    headers.append("v_br");
    headers.append("v_op");
    headers.append("dark_current");
    headers.append("afe type");
    headers.append("afe serial number");

    QString serialNumber = queryParameters->value("serialNumber").toString();
    QVariant serialNumberOrNull = serialNumber.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : serialNumber; //Proponowane zamiaste tej przestażałej wersji rozwiązania nie działają

    QString country = queryParameters->value("country").toString();
    QVariant countryOrNull = country.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : country;

    QString status = queryParameters->value("status").toString();
    QVariant statusOrNull = status.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : status;

    QString institution = queryParameters->value("institution").toString();
    QVariant institutionOrNull = institution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : institution;

    QString sipmModel = queryParameters->value("sipmModel").toString();
    QVariant sipmModelOrNull = sipmModel.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : sipmModel;

    QString room = queryParameters->value("room").toString();
    QVariant roomOrNull = room.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : room;

    QDateTime purchaseDateFrom = queryParameters->value("purchaseDateFrom").toDateTime();

    QDateTime purchaseDateTo = queryParameters->value("purchaseDateTo").toDateTime();

    double vBrFrom = queryParameters->value("vBrFrom").toDouble();

    double vBrTo = queryParameters->value("vBrTo").toDouble();

    double vOpFrom = queryParameters->value("vOpFrom").toDouble();

    double vOpTo = queryParameters->value("vOpTo").toDouble();

    double darkCurrentFrom = queryParameters->value("darkCurrentFrom").toDouble();

    double darkCurrentTo = queryParameters->value("darkCurrentTo").toDouble();

    QString afeSerialNumber = queryParameters->value("afeSerialNumber").toString();
    QVariant afeSerialNumberOrNull = afeSerialNumber.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : afeSerialNumber;

    QSqlQuery * query = nullptr;
    int queryType = queryParameters->value("afeType").toInt();
    if(queryType == Repository::ALL_AFE){
        query = preparedAllAfeSipmQuery;
    }
    else if(queryType == Repository::EXT_AFE)
    {
        query = preparedExtAfeSipmQuery;
    }
    else if(queryType == Repository::MAIN_AFE)
    {
        query = preparedMainAfeSipmQuery;
    }
    else
    {
        throw std::invalid_argument("Invalid query type");
    }

    Repository::SipmQueryParameters * sipmParameters = new Repository::SipmQueryParameters
    {
            serialNumberOrNull,
            countryOrNull,
            statusOrNull,
            institutionOrNull,
            sipmModelOrNull,
            roomOrNull,
            purchaseDateFrom,
            purchaseDateTo,
            vBrFrom,
            vBrTo,
            vOpFrom,
            vOpTo,
            darkCurrentFrom,
            darkCurrentTo,
            afeSerialNumberOrNull
    };
    addSipmQueryResultToModel(model, query, sipmParameters);

    model->setColumnCount(headers.size());
    model->setHorizontalHeaderLabels(headers);

}

bool Repository::saveSipmModelData(const SipmModelDto * sipmModel) const
{
    preparedSipmModelInsertingQuery->bindValue(":name", sipmModel->getName());
    preparedSipmModelInsertingQuery->bindValue(":overvoltage",sipmModel->getOvervoltage());
    if(!preparedSipmModelInsertingQuery->exec())
    {
        qDebug() << "Failed to execute query inserting data into sipm_model table";
        return false;
    }
    return true;
}

bool Repository::saveSipmData(const SipmDto * sipmData) const
{
    if(mcordDatabase->transaction())
    {
        preparedDeviceInsertingQuery->bindValue(":serial_number", sipmData->getSerialNumber());
        preparedDeviceInsertingQuery->bindValue(":purchase_date", sipmData->getPurchaseDateMsc());
        preparedDeviceInsertingQuery->bindValue(":comments", sipmData->getComments());
        Location location = sipmData->getLocation();
        preparedDeviceInsertingQuery->bindValue(":country", location.getCountry());
        preparedDeviceInsertingQuery->bindValue(":institution", location.getInstitution());
        preparedDeviceInsertingQuery->bindValue(":room", location.getRoom());
        if(preparedDeviceInsertingQuery->exec())
        {
            preparedDeviceInsertingQuery->first();
            qint64 id = preparedDeviceInsertingQuery->value("id").toLongLong();
            qint64 dateFrom = preparedDeviceInsertingQuery->value("date_from").toLongLong();

            double vBr = sipmData->getVBr();
            double vOp = sipmData->getVOp();
            double darkCurrent = sipmData->getDarkCurrent();
            QString modelName = sipmData->getModel();

            preparedSipmInsertingQuery->bindValue(":id", id);
            preparedSipmInsertingQuery->bindValue(":date_from", dateFrom);
            preparedSipmInsertingQuery->bindValue(":v_br", vBr);
            preparedSipmInsertingQuery->bindValue(":v_op", vOp);
            preparedSipmInsertingQuery->bindValue(":dark_current", darkCurrent);
            preparedSipmInsertingQuery->bindValue(":model_name", modelName);

            if(!preparedSipmInsertingQuery->exec())
            {
                qDebug() << "Failed to execute query inserting data into sipm table";
                if(!mcordDatabase->rollback())
                {
                    qCritical() << "Fail to rollback query inserting data into device table";
                    return false;
                }
            }
        }
        else
        {
            qDebug() << "Failed to execute query inserting data into device table";
            if(!mcordDatabase->rollback())
            {
                qCritical() << "Fail to rollback query inserting data into device table";
                return false;
            }
        }

        preparedDeviceInsertingQuery->finish();
        if(!mcordDatabase->commit())
        {
            qDebug() << "Failed to commit";
            if(!mcordDatabase->rollback())
            {
                qCritical() << "Fail to rollback query inserting data into device and sipm tables";
                return false;
            }
            return false;
        }
    }
    else
    {
        qDebug() << "Failed to start transaction mode";
        return false;
    }

    return true;
}

void Repository::addScintillatorQueryResultToModel(QStandardItemModel * model, QSqlQuery * query, const ScintillatorQueryParameters * scintillatorParameters) const
{

    query->bindValue(":serialNumber", scintillatorParameters->serialNumberOrNull);
    query->bindValue(":purchaseDateFrom", QVariant(scintillatorParameters->purchaseDateFrom.toMSecsSinceEpoch()));
    query->bindValue(":purchaseDateTo", QVariant(scintillatorParameters->purchaseDateTo.toMSecsSinceEpoch()));
    query->bindValue(":country", scintillatorParameters->countryOrNull);
    query->bindValue(":status", scintillatorParameters->statusOrNull);
    query->bindValue(":institution", scintillatorParameters->institutionOrNull);
    query->bindValue(":model", scintillatorParameters->scintillatorModelOrNull);
    query->bindValue(":room", scintillatorParameters->roomOrNull);

    if(query->exec())
    {
        while(query->next())
        {
            qint64 purchaseDateMsc = query->value("purchase_date").toLongLong();
            QString purchaseDateText = "";
            if(purchaseDateMsc != 0)
            {
                purchaseDateText = QDateTime::fromMSecsSinceEpoch(purchaseDateMsc).toString("dd.MM.yyyy");
            }

            QList<QStandardItem*> rowList = {
                new QStandardItem("Scintillator"),
                new QStandardItem(preparedScintillatorQuery->value("serial_number").toString()),
                new QStandardItem(purchaseDateText),
                new QStandardItem(preparedScintillatorQuery->value("model").toString()),
                new QStandardItem(preparedScintillatorQuery->value("status").toString()),
                new QStandardItem(preparedScintillatorQuery->value("country").toString()),
                new QStandardItem(preparedScintillatorQuery->value("institution").toString()),
                new QStandardItem(preparedScintillatorQuery->value("room").toString()),
                new QStandardItem(preparedScintillatorQuery->value("comment").toString())
            };
            model->appendRow(rowList);
        }
    }
    else
    {
        qDebug() << "Error! The Scintillator query has failed  " << query->lastError() << query->boundValues();
    }
}


void Repository::addDataToModelScintillator(QStandardItemModel * model, QHash<QString, QVariant> * queryParameters) const
{
    model->clear();

    //TODO Nazwy kolumn przenieść do stałej i utworzyć jakieś miejsce do przechowywania tego typu informacji.
    // Docelowo wdrożyć narzędzie umożliwiające wielojęzyczność Qt Linguist
    QStringList headers;
    headers.append("Type");
    headers.append("Serial number");
    headers.append("Purchase date");
    headers.append("Model");
    headers.append("Status");
    headers.append("Country");
    headers.append("Institution");
    headers.append("Room");
    headers.append("Comment");

    QString serialNumber = queryParameters->value("serialNumber").toString();
    QVariant serialNumberOrNull = serialNumber.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : serialNumber; //Proponowane zamiaste tej przestażałej wersji rozwiązania nie działają

    QString country = queryParameters->value("country").toString();
    QVariant countryOrNull = country.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : country;

    QString status = queryParameters->value("status").toString();
    QVariant statusOrNull = status.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : status;

    QString institution = queryParameters->value("institution").toString();
    QVariant institutionOrNull = institution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : institution;

    QString room = queryParameters->value("room").toString();
    QVariant roomOrNull = room.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : room;

    QString scintillatorModel = queryParameters->value("scintillatorModel").toString();
    QVariant scintillatorModelOrNull = scintillatorModel.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : scintillatorModel;

    QDateTime purchaseDateFrom = queryParameters->value("purchaseDateFrom").toDateTime();

    QDateTime purchaseDateTo = queryParameters->value("purchaseDateTo").toDateTime();

    Repository::ScintillatorQueryParameters * scintillatorParameters = new Repository::ScintillatorQueryParameters
    {
            serialNumberOrNull,
            countryOrNull,
            statusOrNull,
            institutionOrNull,
            scintillatorModelOrNull,
            roomOrNull,
            purchaseDateFrom,
            purchaseDateTo
    };
    addScintillatorQueryResultToModel(model, preparedScintillatorQuery, scintillatorParameters);

    model->setColumnCount(headers.size());
    model->setHorizontalHeaderLabels(headers);
}

QSqlQuery * Repository::createQuery(QSqlDatabase * mcordDatabase, QString queryString)
{
    QSqlQuery * query = new QSqlQuery(mcordDatabase->database());
    query->prepare(queryString);
    return query;
}

void Repository::removeQuery(QSqlQuery * query)
{
    delete query;
    query = nullptr;
}

void Repository::addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName) const
{
    if(query->exec())
    {
       while(query->next())
       {
        list->append(query->value(columnName).toString());
       }
    }
    else
    {
        qDebug() << "Error! The query has failed" << query->lastError() << query->boundValues();

    }
}

void Repository::addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName, QHash<QString, QVariant> parameters) const
{

    QHash<QString, QVariant>::iterator i;
    for (i = parameters.begin(); i != parameters.end(); ++i)
    {
        query->bindValue(i.key(), i.value());
    }

    if(query->exec())
    {
       while(query->next())
       {
        list->append(query->value(columnName).toString());
       }
    }
    else
    {
        qDebug() << "Error! The query has failed" << query->lastError() << query->boundValues();

    }
}


