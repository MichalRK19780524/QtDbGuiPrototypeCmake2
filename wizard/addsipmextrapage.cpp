#include "addsipmextrapage.h"
#include "ui_addsipmextrapage.h"

AddSipmExtraPage::AddSipmExtraPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::AddSipmExtraPage)
{
    ui->setupUi(this);
}

AddSipmExtraPage::~AddSipmExtraPage()
{
    delete ui;
}
