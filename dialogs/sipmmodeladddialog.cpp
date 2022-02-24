#include "sipmmodeladddialog.h"
#include "ui_sipmmodeladddialog.h"

SipmModelAddDialog::SipmModelAddDialog(const Repository * repo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SipmModelAddDialog),
    repository(repo)
{
    ui->setupUi(this);
}

SipmModelAddDialog::~SipmModelAddDialog()
{
    delete ui;
}

SipmModelDto SipmModelAddDialog::getSipmModelData() const
{
    QString name = ui->lineEdit_name->text();
    double overvoltage = ui->doubleSpinBox_overvoltage->value();

    return SipmModelDto(name, overvoltage);
}

void SipmModelAddDialog::on_buttonBox_accepted()
{
    const SipmModelDto data = getSipmModelData();
    repository->saveSipmModelData(&data);
}

