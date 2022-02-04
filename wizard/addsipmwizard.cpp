#include "addsipmwizard.h"
#include "addsipmbasicpage.h"
#include "addsipmextrapage.h"

AddSipmWizard::AddSipmWizard(QStandardItemModel *model, QWidget *parent)
    : QWizard{parent}
{
    setPage(ADD_BASIC, new AddSipmBasicPage);
    setPage(ADD_EXTRA, new AddSipmExtraPage);
    setStartId(ADD_BASIC);

    setWizardStyle(ModernStyle);
}
