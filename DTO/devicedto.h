#ifndef DEVICEDTO_H
#define DEVICEDTO_H

#include <QString>
#include <QDate>

class DeviceDto
{
public:

    DeviceDto(const QString &serialNumber, const QDate &purchaseDate, const QString &comments) :
        serialNumber(serialNumber),
        purchaseDate(purchaseDate),
        comments(comments)
    {}

    qint64 getPurchaseDateMsc() const
    {
        return (purchaseDate.startOfDay().currentMSecsSinceEpoch() + purchaseDate.endOfDay().currentMSecsSinceEpoch())/2;
    }

    const QString & getSerialNumber() const
    {
        return serialNumber;
    }

    const QString & getComments() const
    {
        return comments;
    }

private:
    QString serialNumber;
    QDate purchaseDate;
    QString comments;
};

#endif // DEVICEDTO_H
