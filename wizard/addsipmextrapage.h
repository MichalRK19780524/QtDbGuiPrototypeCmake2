#ifndef ADDSIPMEXTRAPAGE_H
#define ADDSIPMEXTRAPAGE_H

#include <QWizardPage>

namespace Ui {
class AddSipmExtraPage;
}

class AddSipmExtraPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit AddSipmExtraPage(QWidget *parent = nullptr);
    ~AddSipmExtraPage();

private:
    Ui::AddSipmExtraPage *ui;
};

#endif // ADDSIPMEXTRAPAGE_H
