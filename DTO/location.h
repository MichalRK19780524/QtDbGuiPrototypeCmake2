#ifndef LOCATION_H
#define LOCATION_H

#include <QString>

class Location
{
public:
    Location(const QString &country, const QString &institution, const QString &room) :
        country(country),
        institution(institution),
        room(room)
    {}

    const QString & getCountry() const
    {
        return country;
    }

    const QString & getInstitution() const
    {
        return institution;
    }

    const QString & getRoom() const
    {
        return room;
    }

private:
    QString country;
    QString institution;
    QString room;
};

#endif // LOCATION_H
