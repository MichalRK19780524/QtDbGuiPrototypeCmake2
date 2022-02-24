#ifndef DYNAMICQCOMBOBOX_H
#define DYNAMICQCOMBOBOX_H

#include <QComboBox>
#include <QObject>

class DynamicQComboBox : public QComboBox
{
    Q_OBJECT
public:
    DynamicQComboBox(QWidget *parent = nullptr);

    // QComboBox interface
public:
    void showPopup() override;
};

#endif // DYNAMICQCOMBOBOX_H
