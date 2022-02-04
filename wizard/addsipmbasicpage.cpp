#include "addsipmbasicpage.h"
#include "addsipmwizard.h"
#include "ui_addsipmbasicpage.h"

AddSipmBasicPage::AddSipmBasicPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::AddSipmBasicPage)
{
    ui->setupUi(this);
}

AddSipmBasicPage::~AddSipmBasicPage()
{
    delete ui;
}

int AddSipmBasicPage::nextId() const
{
        return AddSipmWizard::ADD_EXTRA;

}

void AddSipmBasicPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);
}
