#include "addsipmbasicpage.h"
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
