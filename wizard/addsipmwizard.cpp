#include "addsipmwizard.h"
#include "addsipmbasicpage.h"
#include "addsipmextrapage.h"
#include <QDebug>

AddSipmWizard::AddSipmWizard(const Repository *repo, QWidget *parent)
    : QWizard{parent}, repository(repo)
{
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch  << QWizard::CancelButton << QWizard::FinishButton;
    setButtonLayout(layout);

    setWizardStyle(ModernStyle);

    QWizardPage * basePage = new AddSipmBasicPage(repository);
    setPage(ADD_BASIC, basePage);
//    setPage(ADD_EXTRA, new AddSipmExtraPage);
    setStartId(ADD_BASIC);

}

void AddSipmWizard::accept()
{
    AddSipmBasicPage * basicPage = qobject_cast<AddSipmBasicPage *>(this->page(ADD_BASIC));
    const SipmDto data = basicPage->getSipmData();
    repository->saveSipmData(&data);
    QDialog::accept();
}
