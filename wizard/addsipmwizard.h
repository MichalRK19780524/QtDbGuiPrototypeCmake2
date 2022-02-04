#ifndef ADDSIPMWIZARD_H
#define ADDSIPMWIZARD_H

#include <QWizard>
#include <QStandardItemModel>

class AddSipmWizard : public QWizard
{
    Q_OBJECT
public:
    explicit AddSipmWizard(QStandardItemModel *model, QWidget *parent = nullptr);
    enum {ADD_BASIC, ADD_EXTRA};


signals:

};

#endif // ADDSIPMWIZARD_H
