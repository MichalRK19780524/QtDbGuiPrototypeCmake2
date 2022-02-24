#include "widget.h"
#include "ui_widget.h"
#include "wizard/addsipmwizard.h"

#include <QDateTime>
#include <QCompleter>


Widget::Widget(Repository * repo, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget),
      repository(repo)
{
    ui->setupUi(this);



    //TODO Utworzyć w bazie danych tabelkę (chyba jedna wystarczy) w której będą trzymane takie dane jak lista dostępnych państw, instytucji, pokojów, możliwych stanów urządzeń, itp.,
    //tak, żeby można było je wczytać przy starcie aplikacji.
//    QStringList countries = {"", "Poland", "Russia"}; //Zapoznać się z Qt Linguist, tak, żeby w programie i bazie danych były angielskie nazwy, a inne języki były uwzględnione jedynie w dedykowanym do tego narzędziu.

    countries = new QStringList(); //Lista krajów jest taka sama dla wszstkich urządzeń i nie zależy od od stanu aplikacji
    countries->append("");

    repository->addDataToStringList(countries, repository->getCountryComboBoxQuery(), "name");
    ui->comboBox_country->addItems(*countries);
    ui->comboBox_countryScintillator->addItems(*countries);

    QStringList sipmInstitutions{""};

    QString selectedCountrySipm = ui->comboBox_country->currentText();
    QVariant selectedCountrySipmOrNull = selectedCountrySipm.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedCountrySipm;

    QHash<QString, QVariant> institutionSipmComboBoxQueryParameters{std::make_pair(":country", selectedCountrySipmOrNull)};

    repository->addDataToStringList(&sipmInstitutions, repository->getInstitutionComboBoxQuery(), "name", institutionSipmComboBoxQueryParameters);
    ui->comboBox_institution->clear();
    ui->comboBox_institution->addItems(sipmInstitutions);

    QStringList scintillatorInstitutions({""});
    QString selectedCountryScintillator = ui->comboBox_countryScintillator->currentText();
    QVariant selectedCountryScintillatorOrNull = selectedCountryScintillator.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedCountrySipm;

    QHash<QString, QVariant> institutionScintillatorComboBoxQueryParameters{std::make_pair(":country", selectedCountrySipmOrNull)};

    repository->addDataToStringList(&scintillatorInstitutions, repository->getInstitutionComboBoxQuery(), "name", institutionScintillatorComboBoxQueryParameters);
    ui->comboBox_institutionScintillator->clear();
    ui->comboBox_institutionScintillator->addItems(scintillatorInstitutions);

    QString selectedInstitutionSipm = ui->comboBox_institution->currentText();
    QVariant selectedInstitutioSipmOrNull = selectedInstitutionSipm.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedInstitutionSipm;

    QHash<QString, QVariant> sipmRoomComboBoxQueryParameters
    {
        std::make_pair(":country", selectedCountrySipmOrNull),
        std::make_pair(":institution", selectedInstitutioSipmOrNull)
    };

    QStringList sipmRooms({""});
    repository->addDataToStringList(&sipmRooms, repository->getRoomNoComboBoxQuery(), "name", sipmRoomComboBoxQueryParameters);
    ui->comboBox_room->clear();
    ui->comboBox_room->addItems(sipmRooms);

    QString selectedInstitutionScintillator = ui->comboBox_institutionScintillator->currentText();
    QVariant selectedInstitutionScintillatorOrNull = selectedInstitutionScintillator.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedInstitutionScintillator;

    QHash<QString, QVariant> scintillatorRoomComboBoxQueryParameters
    {
        std::make_pair(":country", selectedCountryScintillatorOrNull),
        std::make_pair(":institution", selectedInstitutionScintillatorOrNull)
    };

    QStringList scintillatorRooms({""});
    repository->addDataToStringList(&scintillatorRooms, repository->getRoomNoComboBoxQuery(), "name", scintillatorRoomComboBoxQueryParameters);
    ui->comboBox_roomScintillator->clear();
    ui->comboBox_roomScintillator->addItems(scintillatorRooms);

    ui->comboBox_status->addItem("", "");
    ui->comboBox_status->addItem("new", "new");
    ui->comboBox_status->addItem("mounted in the bar", "bar");

    ui->comboBox_statusScintillator->addItem("", "");
    ui->comboBox_statusScintillator->addItem("new", "new");
    ui->comboBox_statusScintillator->addItem("mounted in the bar", "bar");

    QStringList types = {"","SiPM", "AFE", "Hub", "Scintillator"};
    ui->comboBox_type->addItems(types);

    QStringList models{""};
    repository->addDataToStringList(&models, repository->getSipmModelComboBoxQuery(), "name");
    ui->comboBox_model->addItems(models);

    QStringList modelsScintillator = {""};
    repository->addDataToStringList(&modelsScintillator, repository->getScintillatorModelComboBoxQuery(), "name");
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

    allAfeSerialNumberList = repository->getAllAfeSerialNumberList();
    allAfeCompleter = new QCompleter(*allAfeSerialNumberList, this);
    ui->comboBox_afeSerialNumber->setCompleter(allAfeCompleter);
    ui->comboBox_afeSerialNumber->addItems(*allAfeSerialNumberList);

    ui->comboBox_afeSerialNumber->setInsertPolicy(QComboBox::NoInsert);

    mainAfeSerialNumberList = repository->getMainAfeSerialNumberList();
    mainAfeCompleter = new QCompleter(*mainAfeSerialNumberList, this);

    extAfeSerialNumberList = repository->getExtAfeSerialNumberList();
    extAfeCompleter = new QCompleter(*extAfeSerialNumberList, this);

    ui->checkBox_afeMain->setCheckState(Qt::Checked);
    ui->checkBox_afeExt->setCheckState(Qt::Checked);

    queryParameters = this->getSipmQueryParameters();

    mcordModelSipm = new QStandardItemModel(this);
    mcordModelScintillator = new QStandardItemModel(this);
    wizard = new AddSipmWizard(repo, this);
}

Widget::~Widget()
{
    delete ui;

    delete countries;

    delete allAfeCompleter;
    delete mainAfeCompleter;
    delete extAfeCompleter;
}


QHash<QString, QVariant> * Widget::getSipmQueryParameters() const
{
    QHash<QString, QVariant> * result = new QHash<QString, QVariant> ();

    QString serialNumber = ui->lineEdit_serialNumber->text();
    result->insert("serialNumber", QVariant(serialNumber));
    QString country = ui->comboBox_country->currentText();
    result->insert("country", QVariant(country));
    QString status = ui->comboBox_status->currentData().toString();
    result->insert("status", QVariant(status));
    QString institution = ui->comboBox_institution->currentText();
    result->insert("institution", QVariant(institution));
    QString deviceModel = ui->comboBox_model->currentText();
    result->insert("deviceModel", QVariant(deviceModel));
    QString room = ui->comboBox_room->currentText();
    result->insert("room", QVariant(room));
    QDateTime purchaseDateFrom = ui->dateEdit_fromPurchaseDate->date().startOfDay();
    result->insert("purchaseDateFrom", QVariant(purchaseDateFrom));
    QDateTime purchaseDateTo = ui->dateEdit_toPurchaseDate->date().endOfDay();
    result->insert("purchaseDateTo", QVariant(purchaseDateTo));
    double vBrFrom = ui->doubleSpinBox_vBrFrom->value();
    result->insert("vBrFrom", QVariant(vBrFrom));
    double vBrTo = ui->doubleSpinBox_vBrTo->value();
    result->insert("vBrTo", QVariant(vBrTo));
    double vOpFrom = ui->doubleSpinBox_vOpFrom->value();
    result->insert("vOpFrom", QVariant(vOpFrom));
    double vOpTo = ui->doubleSpinBox_vOpTo->value();
    result->insert("vOpTo", QVariant(vOpTo));
    double darkCurrentFrom = ui->doubleSpinBox_darkCurrentFrom->value();
    result->insert("darkCurrentFrom", QVariant(darkCurrentFrom));
    double darkCurrentTo = ui->doubleSpinBox_darkCurrentTo->value();
    result->insert("darkCurrentTo", QVariant(darkCurrentTo));
    QString afeSerialNumber = ui->comboBox_afeSerialNumber->currentText();
    result->insert("afeSerialNumber", QVariant(afeSerialNumber));

    if(ui->checkBox_afeExt->isChecked() && ui->checkBox_afeMain->isChecked())
    {
         result->insert("afeType", QVariant(repository->ALL_AFE));
    }
    else if(ui->checkBox_afeExt->isChecked() && !ui->checkBox_afeMain->isChecked())
    {
         result->insert("afeType", QVariant(repository->EXT_AFE));
    }
    else if(!ui->checkBox_afeExt->isChecked() && ui->checkBox_afeMain->isChecked())
    {
         result->insert("afeType", QVariant(repository->MAIN_AFE));
    }

    return result;
}


QHash<QString, QVariant> * Widget::getScintillatorQueryParameters() const
{
    QHash<QString, QVariant> * result = new QHash<QString, QVariant> ();

    QString serialNumber = ui->lineEdit_serialNumberScintillator->text();
    result->insert("serialNumber", QVariant(serialNumber));
    QString country = ui->comboBox_countryScintillator->currentText();
    result->insert("country", QVariant(country));
    QString status = ui->comboBox_statusScintillator->currentData().toString();
    result->insert("status", QVariant(status));
    QString institution = ui->comboBox_institutionScintillator->currentText();
    result->insert("institution", QVariant(institution));
    QString scintillatorModel = ui->comboBox_modelScintillator->currentText();
    result->insert("scintillatorModel", QVariant(scintillatorModel));
    QString room = ui->comboBox_roomScintillator->currentText();
    result->insert("room", QVariant(room));
    QDateTime purchaseDateFrom = ui->dateEdit_fromPurchaseDateScintillator->date().startOfDay();
    result->insert("purchaseDateFrom", QVariant(purchaseDateFrom));
    QDateTime purchaseDateTo = ui->dateEdit_toPurchaseDateScintillator->date().endOfDay();
    result->insert("purchaseDateTo", QVariant(purchaseDateTo));

    return result;
}

void Widget::on_pushButton_search_clicked()
{
    repository->addDataToModelSipm(mcordModelSipm, getSipmQueryParameters());
    ui->tableView_sipm->setModel(mcordModelSipm);
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
    repository->addDataToModelScintillator(mcordModelScintillator, getScintillatorQueryParameters());
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

void Widget::on_comboBox_country_currentTextChanged(const QString &selectedSipmCountry)
{
    QVariant selectedSipmCountryOrNull = selectedSipmCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedSipmCountry;

    QStringList sipmInstitutions({""});
    QHash<QString, QVariant> institutionComboBoxQueryParameters{std::make_pair(":country", selectedSipmCountryOrNull)};

    repository->addDataToStringList(&sipmInstitutions, repository->getInstitutionComboBoxQuery(), "name", institutionComboBoxQueryParameters);
    ui->comboBox_institution->clear();
    ui->comboBox_institution->addItems(sipmInstitutions);
}

void Widget::on_comboBox_institution_currentTextChanged(const QString &selectedSipmInstitution)
{
    QString selectedSipmCountry = ui->comboBox_country->currentText();
    QVariant selectedSipmCountryOrNull = selectedSipmCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedSipmCountry;
    QVariant selectedSipmInstitutioOrNull = selectedSipmInstitution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedSipmInstitution;

    QHash<QString, QVariant> sipmRoomComboBoxQueryParameters
    {
        std::make_pair(":country", selectedSipmCountryOrNull),
        std::make_pair(":institution", selectedSipmInstitutioOrNull)
    };

    QStringList sipmRooms({""});
    repository->addDataToStringList(&sipmRooms, repository->getRoomNoComboBoxQuery(), "name", sipmRoomComboBoxQueryParameters);
    ui->comboBox_room->clear();
    ui->comboBox_room->addItems(sipmRooms);
}


void Widget::on_comboBox_countryScintillator_currentTextChanged(const QString &selectedScintillatorCountry)
{
    QVariant selectedCountryOrNull = selectedScintillatorCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedScintillatorCountry;

    QStringList scintillatorInstitutions{""};
    QHash<QString, QVariant> institutionComboBoxQueryParameters{std::make_pair(":country", selectedCountryOrNull)};

    repository->addDataToStringList(&scintillatorInstitutions, repository->getInstitutionComboBoxQuery(), "name", institutionComboBoxQueryParameters);
    ui->comboBox_institutionScintillator->clear();
    ui->comboBox_institutionScintillator->addItems(scintillatorInstitutions);
}

void Widget::on_comboBox_institutionScintillator_currentTextChanged(const QString &selectedScintillatorInstitution)
{
    QString selectedScintillatorCountry = ui->comboBox_countryScintillator->currentText();
    QVariant selectedScintillatorCountryOrNull = selectedScintillatorCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedScintillatorCountry;
    QVariant selectedScintillatorInstitutioOrNull = selectedScintillatorInstitution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedScintillatorInstitution;

    QHash<QString, QVariant> scintillatorRoomComboBoxQueryParameters
    {
        std::make_pair(":country", selectedScintillatorCountryOrNull),
        std::make_pair(":institution", selectedScintillatorInstitutioOrNull)
    };

    QStringList scintillatorRooms({""});
    repository->addDataToStringList(&scintillatorRooms, repository->getRoomNoComboBoxQuery(), "name", scintillatorRoomComboBoxQueryParameters);
    ui->comboBox_roomScintillator->clear();
    ui->comboBox_roomScintillator->addItems(scintillatorRooms);
}

