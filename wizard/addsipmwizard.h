#ifndef ADDSIPMWIZARD_H
#define ADDSIPMWIZARD_H
#include "services/repository.h"

#include <QWizard>
#include <QStandardItemModel>

class AddSipmWizard : public QWizard
{
    Q_OBJECT
public:
    explicit AddSipmWizard(const Repository * repo, QWidget * parent = nullptr);
    enum {ADD_BASIC, ADD_EXTRA};

    void accept() override;
private:
    const Repository * repository;
};

#endif // ADDSIPMWIZARD_H
