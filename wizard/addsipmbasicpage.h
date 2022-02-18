#ifndef ADDSIPMBASICPAGE_H
#define ADDSIPMBASICPAGE_H
#include "services/repository.h"
#include "DTO/sipmdto.h"

#include <QWizardPage>


namespace Ui {
class AddSipmBasicPage;
}

class AddSipmBasicPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit AddSipmBasicPage(const Repository * repo, QWidget *parent = nullptr);
    ~AddSipmBasicPage();
    int nextId() const override;

    const SipmDto getSipmData() const;

private slots:
    void on_comboBox_country_currentTextChanged(const QString &selectedCountry);

    void on_comboBox_institution_currentTextChanged(const QString &arg1);

private:
    Ui::AddSipmBasicPage *ui;
    const Repository * repository;
};

#endif // ADDSIPMBASICPAGE_H
