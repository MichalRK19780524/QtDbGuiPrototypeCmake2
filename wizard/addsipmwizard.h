#ifndef ADDSIPMWIZARD_H
#define ADDSIPMWIZARD_H

#include <QWizard>
#include <QStandardItemModel>

class AddSipmWizard : public QWizard
{
    Q_OBJECT
public:
    explicit AddSipmWizard(QStandardItemModel *model, QWidget *parent = nullptr);
    enum {Add_Basic, Add_Links};


signals:

};

#endif // ADDSIPMWIZARD_H
