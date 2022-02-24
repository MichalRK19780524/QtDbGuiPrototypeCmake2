#ifndef SIPMMODEL_H
#define SIPMMODEL_H

#include <QString>

class SipmModelDto
{
public:

    SipmModelDto(const QString & name, const double & overvoltage):
    name(name),
    overvoltage(overvoltage)
    {
    }

    const QString & getName() const
    {
        return name;
    }

    double getOvervoltage() const
    {
        return overvoltage;
    }



private:
    QString name;
    double overvoltage;
};

#endif // SIPMMODEL_H
