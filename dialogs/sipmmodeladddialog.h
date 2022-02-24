#ifndef SIPMMODELADDDIALOG_H
#define SIPMMODELADDDIALOG_H

#include "DTO/sipmmodeldto.h"
#include "services/repository.h"

#include <QDialog>

namespace Ui {
class SipmModelAddDialog;
}

class SipmModelAddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SipmModelAddDialog(const Repository * repo, QWidget *parent = nullptr);
    ~SipmModelAddDialog();

    SipmModelDto getSipmModelData() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SipmModelAddDialog *ui;
    const Repository * repository;
};

#endif // SIPMMODELADDDIALOG_H
