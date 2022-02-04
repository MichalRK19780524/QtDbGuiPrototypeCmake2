#include "widget.h"
#include "ui_widget.h"
#include "wizard/addsipmwizard.h"

#include <QDateTime>
#include <QCompleter>

const QString Widget::allAfeSipmFilteredQueryString =
R"(SELECT dsv.serial_number  AS serial_number,
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

const QString Widget::mainAfeSipmFilteredQueryString =
R"(SELECT dsv.serial_number  AS serial_number,
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

const QString Widget::extAfeSipmFilteredQueryString =
R"(SELECT dsv.serial_number  AS serial_number,
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

const QString Widget::allAfeComboBoxQueryString =
R"(SELECT serial_number FROM device_afe_main_view
    UNION
    SELECT serial_number FROM device_afe_ext_view daev)";

const QString Widget::mainAfeComboBoxQueryString =
R"(SELECT serial_number FROM device_afe_main_view)";

const QString Widget::extAfeComboBoxQueryString =
R"(SELECT serial_number FROM device_afe_ext_view daev)";

const QString Widget::countryComboBoxQueryString =
        R"(SELECT name FROM country)";

const QString Widget::institutionComboBoxQueryString =
        R"(SELECT i.name
           FROM institution i
           LEFT JOIN country c ON i.country_id = c.id
           WHERE c.name = :country)";

const QString Widget::roomNoComboBoxQueryString =
        R"(SELECT DISTINCT room
           FROM location
           WHERE country = :country
        )";

const QString Widget::scintillatorFilteredQueryString =
R"(SELECT d.serial_number AS serial_number,
       d.purchase_date AS purchase_date,
       d.model 		   AS model,
       d.status        AS status,
       d.comments 	   AS comment,
       l.institution   AS institution,
       l.country       AS country,
       l.room          AS room
FROM location l
JOIN device d ON d.location_id = l.id
JOIN scintillator s ON d.id = s.id AND d.date_from = s.date_from
WHERE (d.serial_number = coalesce(:serialNumber, d.serial_number) OR d.serial_number ISNULL)
    AND (d.purchase_date BETWEEN :purchaseDateFrom AND :purchaseDateTo OR d.purchase_date ISNULL)
    AND (l.country = coalesce(:country, l.country) OR l.country  ISNULL)
    AND (d.status = coalesce(:status, d.status) OR d.status ISNULL)
    AND (l.institution = coalesce(:institution, l.institution) OR l.institution  ISNULL)
    AND (d.model = coalesce(:model, d.model) OR d.model ISNULL)
    AND (l.room = coalesce(:room, l.room) OR l.room  ISNULL))";


void Widget::openDatabase()
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

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    openDatabase();
    mcordModelSipm = new QStandardItemModel(this);
    mcordModelScintillator = new QStandardItemModel(this);
    //TODO Utworzyć w bazie danych tabelkę (chyba jedna wystarczy) w której będą trzymane takie dane jak lista dostępnych państw, instytucji, pokojów, możliwych stanów urządzeń, itp.,
    //tak, żeby można było je wczytać przy starcie aplikacji.
//    QStringList countries = {"", "Poland", "Russia"}; //Zapoznać się z Qt Linguist, tak, żeby w programie i bazie danych były angielskie nazwy, a inne języki były uwzględnione jedynie w dedykowanym do tego narzędziu.
    QSqlQuery * countryComboBoxQuery = createQuery(mcordDatabase, countryComboBoxQueryString);
    countries = new QStringList();
    countries->append("");
    addDataToStringList(countries, countryComboBoxQuery, "name");
    removeQuery(countryComboBoxQuery);
    ui->comboBox_country->addItems(*countries);
    ui->comboBox_countryScintillator->addItems(*countries);

    QStringList institutions = {"", "NCBJ", "PW", "JINR"};
    ui->comboBox_institution->addItems(institutions);
    ui->comboBox_institutionScintillator->addItems(institutions);

    QStringList rooms = {"", "40", "238"};
    ui->comboBox_room->addItems(rooms);
    ui->comboBox_roomScintillator->addItems(rooms);

//    QStringList statuses = {"nowe", "zamontowano w belce"};
    ui->comboBox_status->addItem("", "");
    ui->comboBox_status->addItem("new", "nowe");
    ui->comboBox_status->addItem("mounted in the bar", "zamontowano w belce");

    ui->comboBox_statusScintillator->addItem("", "");
    ui->comboBox_statusScintillator->addItem("new", "nowe");
    ui->comboBox_statusScintillator->addItem("mounted in the bar", "zamontowano w belce");

    QStringList types = {"","SiPM", "AFE", "Hub", "Scintillator"};
    ui->comboBox_type->addItems(types);

    QStringList models = {"","S13360-3075PE"};
    ui->comboBox_model->addItems(models);

    QStringList modelsScintillator = {""};
    ui->comboBox_modelScintillator->addItems(modelsScintillator);

    ui->dateEdit_fromPurchaseDate->setCalendarPopup(true);
    QDate dateFrom(2015,1,1);
    ui->dateEdit_fromPurchaseDate->setDate(dateFrom);

    ui->dateEdit_fromPurchaseDateScintillator->setCalendarPopup(true);
    ui->dateEdit_fromPurchaseDateScintillator->setDate(dateFrom);

    ui->dateEdit_toPurchaseDate->setCalendarPopup(true);
    ui->dateEdit_toPurchaseDate->setDate(QDate::currentDate());

    ui->dateEdit_toPurchaseDateScintillator->setCalendarPopup(true);
    ui->dateEdit_toPurchaseDateScintillator->setDate(QDate::currentDate());

    ui->doubleSpinBox_vBrFrom->setValue(vBrFrom);
    ui->doubleSpinBox_vBrTo->setValue(vBrTo);

    ui->doubleSpinBox_vOpFrom->setValue(vOpFrom);
    ui->doubleSpinBox_vOpTo->setValue(vOpTo);

    ui->doubleSpinBox_darkCurrentFrom->setValue(darkCurrentFrom);
    ui->doubleSpinBox_darkCurrentTo->setValue(darkCurrentTo);

    ui->comboBox_afeSerialNumber->setEditable(true);

    QSqlQuery * allAfeComboBoxQuery = createQuery(mcordDatabase, allAfeComboBoxQueryString);
    allAfeSerialNumberList = new QStringList();
    allAfeSerialNumberList->append("");
    addDataToStringList(allAfeSerialNumberList, allAfeComboBoxQuery, "serial_number");
    removeQuery(allAfeComboBoxQuery);

    allAfeCompleter = new QCompleter(*allAfeSerialNumberList, this);
    ui->comboBox_afeSerialNumber->setCompleter(allAfeCompleter);
    ui->comboBox_afeSerialNumber->addItems(*allAfeSerialNumberList);
    ui->comboBox_afeSerialNumber->setInsertPolicy(QComboBox::NoInsert);

    QSqlQuery * mainAfeComboBoxQuery = createQuery(mcordDatabase, mainAfeComboBoxQueryString);
    mainAfeSerialNumberList = new QStringList();
    mainAfeSerialNumberList->append("");
    addDataToStringList(mainAfeSerialNumberList, mainAfeComboBoxQuery, "serial_number");
    removeQuery(mainAfeComboBoxQuery);
    mainAfeCompleter = new QCompleter(*mainAfeSerialNumberList, this);

    QSqlQuery * extAfeComboBoxQuery = createQuery(mcordDatabase, extAfeComboBoxQueryString);
    extAfeSerialNumberList = new QStringList();
    extAfeSerialNumberList->append("");
    addDataToStringList(extAfeSerialNumberList, extAfeComboBoxQuery, "serial_number");
    removeQuery(extAfeComboBoxQuery);
    extAfeCompleter = new QCompleter(*mainAfeSerialNumberList, this);

    ui->checkBox_afeMain->setCheckState(Qt::Checked);
    ui->checkBox_afeExt->setCheckState(Qt::Checked);

    preparedAllAfeSipmQuery = createQuery(mcordDatabase, allAfeSipmFilteredQueryString);
    preparedMainAfeSipmQuery = createQuery(mcordDatabase, mainAfeSipmFilteredQueryString);
    preparedExtAfeSipmQuery = createQuery(mcordDatabase, extAfeSipmFilteredQueryString);
    preparedScintillatorQuery = createQuery(mcordDatabase, scintillatorFilteredQueryString);
    wizard = new AddSipmWizard(mcordModelSipm, this);
}

Widget::~Widget()
{
    delete ui;
    removeQuery(preparedAllAfeSipmQuery);
    removeQuery(preparedMainAfeSipmQuery);
    removeQuery(preparedExtAfeSipmQuery);
    removeQuery(preparedScintillatorQuery);
    delete countries;
    delete allAfeSerialNumberList;
    delete mainAfeSerialNumberList;
    delete extAfeSerialNumberList;

    delete allAfeCompleter;
    delete mainAfeCompleter;
    delete extAfeCompleter;
}


void Widget::on_pushButton_search_clicked()
{
    if(mcordDatabase->isOpen())
    {
        qDebug() << "MCORD database is open";
    }
    else
    {
        //TODO Wdrożyć jakieś narzędzie do logowania błędów
        qDebug() << "Error! MCORD database is closed";
    }

    if(ui->checkBox_afeExt->isChecked() && ui->checkBox_afeMain->isChecked())
    {
        addDataToModelSipm(mcordModelSipm, preparedAllAfeSipmQuery);
    }
    else if(ui->checkBox_afeExt->isChecked() && !ui->checkBox_afeMain->isChecked())
    {
        addDataToModelSipm(mcordModelSipm, preparedExtAfeSipmQuery);
    }
    else if(!ui->checkBox_afeExt->isChecked() && ui->checkBox_afeMain->isChecked())
    {
        addDataToModelSipm(mcordModelSipm, preparedMainAfeSipmQuery);
    }

    ui->tableView_sipm->setModel(mcordModelSipm);
}

QSqlQuery * Widget::createQuery(QSqlDatabase * mcordDatabase, QString queryString)
{
    QSqlQuery * query = new QSqlQuery(mcordDatabase->database());
    query->prepare(queryString);
    return query;
}

void Widget::removeQuery(QSqlQuery * query)
{
    delete query;
    query = nullptr;
}

void Widget::addDataToModelSipm(QStandardItemModel * model, QSqlQuery * query)
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

    QString serialNumber = ui->lineEdit_serialNumber->text();
    QVariant serialNumberOrNull = serialNumber.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : serialNumber; //Proponowane zamiaste tej przestażałej wersji rozwiązania nie działają

    QString country = ui->comboBox_country->currentText();
    QVariant countryOrNull = country.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : country;

    QString status = ui->comboBox_status->currentData().toString();
    QVariant statusOrNull = status.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : status;
    QString institution = ui->comboBox_institution->currentText();
    QVariant institutionOrNull = institution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : institution;

    QString deviceModel = ui->comboBox_model->currentText();
    QVariant deviceModelOrNull = deviceModel.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : deviceModel;
    QString room = ui->comboBox_room->currentText();
    QVariant roomOrNull = room.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : room;
    qDebug() << room;
    qDebug() << roomOrNull;

    QDateTime purchaseDateFrom = ui->dateEdit_fromPurchaseDate->date().startOfDay();

    qDebug() << purchaseDateFrom;
    qDebug() << purchaseDateFrom.toMSecsSinceEpoch();
    QDateTime purchaseDateTo = ui->dateEdit_toPurchaseDate->date().endOfDay();
    qDebug() << purchaseDateTo;
    qDebug() << purchaseDateTo.toMSecsSinceEpoch();

    double vBrFrom = ui->doubleSpinBox_vBrFrom->value();
    double vBrTo = ui->doubleSpinBox_vBrTo->value();

    double vOpFrom = ui->doubleSpinBox_vOpFrom->value();
    double vOpTo = ui->doubleSpinBox_vOpTo->value();

    double darkCurrentFrom = ui->doubleSpinBox_darkCurrentFrom->value();
    double darkCurrentTo = ui->doubleSpinBox_darkCurrentTo->value();

    QString afeSerialNumber = ui->comboBox_afeSerialNumber->currentText();
    QVariant afeSerialNumberOrNull = afeSerialNumber.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : afeSerialNumber;

    query->bindValue(":serialNumber", serialNumberOrNull);
    query->bindValue(":purchaseDateFrom", QVariant(purchaseDateFrom.toMSecsSinceEpoch()));
    query->bindValue(":purchaseDateTo", QVariant(purchaseDateTo.toMSecsSinceEpoch()));
    query->bindValue(":country", countryOrNull);
    query->bindValue(":status", statusOrNull);
    query->bindValue(":institution", institutionOrNull);
    query->bindValue(":model", deviceModelOrNull);
    query->bindValue(":room", roomOrNull);
    query->bindValue(":vBrFrom", vBrFrom);
    query->bindValue(":vBrTo", vBrTo);
    query->bindValue(":vOpFrom", vOpFrom);
    query->bindValue(":vOpTo", vOpTo);
    query->bindValue(":darkCurrentFrom", darkCurrentFrom);
    query->bindValue(":darkCurrentTo", darkCurrentTo);
    query->bindValue(":afeSerialNumber", afeSerialNumberOrNull);
    qDebug() << query->lastQuery();

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
                new QStandardItem(query->value("v_br").toString()),
                new QStandardItem(query->value("v_op").toString()),
                new QStandardItem(query->value("dark_current").toString()),
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

    model->setColumnCount(headers.size());
    model->setHorizontalHeaderLabels(headers);

}

void Widget::addDataToStringList(QStringList * list, QSqlQuery * query, QString columnName)
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
        qDebug() << "Error! The AFE ComboBox query has failed" << query->lastError() << query->boundValues();

    }
}

void Widget::addDataToModelScintillator(QStandardItemModel * model, QSqlQuery * query)
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

    QString serialNumber = ui->lineEdit_serialNumberScintillator->text();
    QVariant serialNumberOrNull = serialNumber.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : serialNumber; //Proponowane zamiaste tej przestażałej wersji rozwiązania nie działają

    QString country = ui->comboBox_countryScintillator->currentText();
    QVariant countryOrNull = country.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : country;

    QString status = ui->comboBox_statusScintillator->currentData().toString();
    QVariant statusOrNull = status.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : status;
//    qDebug() << statusOrNull;
    QString institution = ui->comboBox_institutionScintillator->currentText();
    QVariant institutionOrNull = institution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : institution;

    QString deviceModel = ui->comboBox_modelScintillator->currentText();
    QVariant deviceModelOrNull = deviceModel.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : deviceModel;
//    qDebug() << deviceModel;
//    qDebug() << deviceModelOrNull;
    QString room = ui->comboBox_roomScintillator->currentText();
    QVariant roomOrNull = room.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : room;
    qDebug() << room;
    qDebug() << roomOrNull;

    QDateTime purchaseDateFrom = ui->dateEdit_fromPurchaseDateScintillator->date().startOfDay();

    qDebug() << purchaseDateFrom;
    qDebug() << purchaseDateFrom.toMSecsSinceEpoch();
    QDateTime purchaseDateTo = ui->dateEdit_toPurchaseDateScintillator->date().endOfDay();
    qDebug() << purchaseDateTo;
    qDebug() << purchaseDateTo.toMSecsSinceEpoch();

     query->bindValue(":serialNumber", serialNumberOrNull);
     query->bindValue(":purchaseDateFrom", QVariant(purchaseDateFrom.toMSecsSinceEpoch()));
     query->bindValue(":purchaseDateTo", QVariant(purchaseDateTo.toMSecsSinceEpoch()));
     query->bindValue(":country", countryOrNull);
     query->bindValue(":status", statusOrNull);
     query->bindValue(":institution", institutionOrNull);
     query->bindValue(":model", deviceModelOrNull);
     query->bindValue(":room", roomOrNull);
     qDebug() << query->lastQuery();
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

            QList<QStandardItem*> rowList =
            {
                new QStandardItem("Scintillator"),
                new QStandardItem(query->value("serial_number").toString()),
                new QStandardItem(purchaseDateText),
                new QStandardItem(query->value("model").toString()),
                new QStandardItem(query->value("status").toString()),
                new QStandardItem(query->value("institution").toString()),
                new QStandardItem(query->value("comment").toString()),
                new QStandardItem(query->value("country").toString()),
                new QStandardItem(query->value("room").toString()),
            };
            model->appendRow(rowList);
         }
     }
     else
     {
         qDebug() << "Error! The Scintillator query has failed  " << query->lastError() << query->boundValues();

     }

     model->setColumnCount(headers.size());
     model->setHorizontalHeaderLabels(headers);

}

void Widget::on_pushButton_vBrReset_clicked()
{
    ui->doubleSpinBox_vBrFrom->setValue(vBrFrom);
    ui->doubleSpinBox_vBrTo->setValue(vBrTo);
}


void Widget::on_pushButton_vOpReset_clicked()
{
    ui->doubleSpinBox_vOpFrom->setValue(vOpFrom);
    ui->doubleSpinBox_vOpTo->setValue(vOpTo);
}


void Widget::on_pushButton_darkCurrentReset_clicked()
{
    ui->doubleSpinBox_darkCurrentFrom->setValue(darkCurrentFrom);
    ui->doubleSpinBox_darkCurrentTo->setValue(darkCurrentTo);
}


void Widget::on_pushButton_searchScintillator_clicked()
{
    if(mcordDatabase->isOpen())
    {
        qDebug() << "MCORD database is open";
    }
    else
    {
        //TODO Wdrożyć jakieś narzędzie do logowania błędów
        qDebug() << "Error! MCORD database is closed";
    }


    addDataToModelScintillator(mcordModelScintillator, preparedScintillatorQuery);
    ui->tableViewScintillator->setModel(mcordModelScintillator);
}


void Widget::on_pushButton_add_clicked()
{
    wizard->show();
}


void Widget::on_checkBox_afeMain_stateChanged(int state)
{
    bool isAfeExtChecked = ui->checkBox_afeExt->isChecked();

    if(state == Qt::Unchecked)
    {
        ui->comboBox_afeSerialNumber->setCompleter(extAfeCompleter);
        ui->comboBox_afeSerialNumber->clear();
        ui->comboBox_afeSerialNumber->addItems(*extAfeSerialNumberList);
        ui->checkBox_afeExt->setCheckState(Qt::Checked);
    }
    else
    {
        if(isAfeExtChecked)
        {
            ui->comboBox_afeSerialNumber->setCompleter(allAfeCompleter);
            ui->comboBox_afeSerialNumber->clear();
            ui->comboBox_afeSerialNumber->addItems(*allAfeSerialNumberList);
        }
        else
        {
            ui->comboBox_afeSerialNumber->setCompleter(mainAfeCompleter);
            ui->comboBox_afeSerialNumber->clear();
            ui->comboBox_afeSerialNumber->addItems(*mainAfeSerialNumberList);
        }
    }
}


void Widget::on_checkBox_afeExt_stateChanged(int state)
{
    bool isAfeMainChecked = ui->checkBox_afeMain->isChecked();

    if(state == Qt::Unchecked)
    {
        ui->comboBox_afeSerialNumber->setCompleter(mainAfeCompleter);
        ui->comboBox_afeSerialNumber->clear();
        ui->comboBox_afeSerialNumber->addItems(*mainAfeSerialNumberList);
        ui->checkBox_afeMain->setCheckState(Qt::Checked);
    }
    else
    {
        if(isAfeMainChecked)
        {
            ui->comboBox_afeSerialNumber->setCompleter(allAfeCompleter);
            ui->comboBox_afeSerialNumber->clear();
            ui->comboBox_afeSerialNumber->addItems(*allAfeSerialNumberList);
        }
        else
        {
            ui->comboBox_afeSerialNumber->setCompleter(extAfeCompleter);
            ui->comboBox_afeSerialNumber->clear();
            ui->comboBox_afeSerialNumber->addItems(*extAfeSerialNumberList);
        }
    }
}

