#include "addsipmbasicpage.h"
#include "addsipmwizard.h"
#include "ui_addsipmbasicpage.h"
#include "services/repository.h"
#include <QPushButton>

AddSipmBasicPage::AddSipmBasicPage(const Repository * repo, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::AddSipmBasicPage),
    repository(repo)
{
    ui->setupUi(this);
    QStringList countries; //Lista krajów jest taka sama dla wszstkich urządzeń i nie zależy od od stanu aplikacji
    repository->addDataToStringList(&countries, repository->getCountryComboBoxQuery(), "name");
    ui->comboBox_country->addItems(countries);

    QStringList institutions;

    QString selectedCountry = ui->comboBox_country->currentText();
    QVariant selectedCountryOrNull = selectedCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedCountry;

    QHash<QString, QVariant> institutionComboBoxQueryParameters{std::make_pair(":country", selectedCountryOrNull)};

    repository->addDataToStringList(&institutions, repository->getInstitutionComboBoxQuery(), "name", institutionComboBoxQueryParameters);
    ui->comboBox_institution->clear();
    ui->comboBox_institution->addItems(institutions);

    QString selectedInstitution = ui->comboBox_institution->currentText();
    QVariant selectedInstitutionOrNull = selectedInstitution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedInstitution;

    QHash<QString, QVariant> roomComboBoxQueryParameters
    {
        std::make_pair(":country", selectedCountryOrNull),
        std::make_pair(":institution", selectedInstitutionOrNull)
    };

    QStringList rooms;
    repository->addDataToStringList(&rooms, repository->getRoomNoComboBoxQuery(), "name", roomComboBoxQueryParameters);
    ui->comboBox_room->clear();
    ui->comboBox_room->addItems(rooms);

    QStringList models;
    repository->addDataToStringList(&models, repository->getSipmModelComboBoxQuery(), "name");
    ui->comboBox_model->clear();
    ui->comboBox_model->addItems(models);

    setButtonText(QWizard::FinishButton, tr("&Save"));
}

AddSipmBasicPage::~AddSipmBasicPage()
{
    delete ui;
}

int AddSipmBasicPage::nextId() const
{
    return -1;
}

const SipmDto AddSipmBasicPage::getSipmData() const
{
    QString serialNumber = ui->lineEdit_serialNumber->text();
    QDate purchaseDate = ui->dateEdit_purchaseDate->date();
    QString comments = ui->textEdit_comments->toPlainText();
    DeviceDto deviceDto(serialNumber, purchaseDate, comments);

    QString country = ui->comboBox_country->currentText();
    QString institution = ui->comboBox_institution->currentText();
    QString room = ui->comboBox_room->currentText();
    Location location(country, institution, room);

    QString model = ui->comboBox_model->currentText();

    double vBr = ui->doubleSpinBox_vBr->value();
    double darkCurrent = ui->doubleSpinBox_darkCurrent->value();

    return SipmDto(deviceDto, location, model, vBr, darkCurrent);
}

void AddSipmBasicPage::on_comboBox_country_currentTextChanged(const QString &selectedCountry)
{
    QVariant selectedCountryOrNull = selectedCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedCountry;

    QStringList institutions;
    QHash<QString, QVariant> institutionComboBoxQueryParameters{std::make_pair(":country", selectedCountryOrNull)};

    repository->addDataToStringList(&institutions, repository->getInstitutionComboBoxQuery(), "name", institutionComboBoxQueryParameters);
    ui->comboBox_institution->clear();
    ui->comboBox_institution->addItems(institutions);
}


void AddSipmBasicPage::on_comboBox_institution_currentTextChanged(const QString &selectedInstitution)
{
    QString selectedCountry = ui->comboBox_country->currentText();
    QVariant selectedCountryOrNull = selectedCountry.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedCountry;
    QVariant selectedInstitutioOrNull = selectedInstitution.isEmpty() ? QVariant(QMetaType(QMetaType::QString)) : selectedInstitution;

    QHash<QString, QVariant> roomComboBoxQueryParameters
    {
        std::make_pair(":country", selectedCountryOrNull),
        std::make_pair(":institution", selectedInstitutioOrNull)
    };

    QStringList rooms;
    repository->addDataToStringList(&rooms, repository->getRoomNoComboBoxQuery(), "name", roomComboBoxQueryParameters);
    ui->comboBox_room->clear();
    ui->comboBox_room->addItems(rooms);
}


