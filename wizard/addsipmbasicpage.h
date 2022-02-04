#ifndef ADDSIPMBASICPAGE_H
#define ADDSIPMBASICPAGE_H

#include <QWizardPage>

namespace Ui {
class AddSipmBasicPage;
}

class AddSipmBasicPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit AddSipmBasicPage(QWidget *parent = nullptr);
    ~AddSipmBasicPage();

    int nextId() const override;
    void setVisible(bool visible) override;

private:
    Ui::AddSipmBasicPage *ui;
};

#endif // ADDSIPMBASICPAGE_H
