#ifndef SIPMDTO_H
#define SIPMDTO_H

#include "DTO/devicedto.h"
#include "DTO/location.h"

#include <QString>
#include <QDate>

class SipmDto : public DeviceDto
{
public:
    SipmDto(const DeviceDto & deviceDto, const Location & location, const QString &model, const double &vBr, const double &darkCurrent) :
        DeviceDto(deviceDto),
        model(model),
        vBr(vBr),
        darkCurrent(darkCurrent),
        location(location)
    {}


    const QString & getModel() const
    {
        return model;
    }

    const double & getVBr() const
    {
        return vBr;
    }

    const double & getDarkCurrent() const
    {
        return darkCurrent;
    }

    const Location & getLocation() const
    {
        return location;
    }

private:
    QString model;
    double vBr;
    double darkCurrent;
    Location location;
};

#endif // SIPMDTO_H
