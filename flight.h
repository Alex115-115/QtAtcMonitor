#pragma once

#include <QString>
#include <vector>
#include <QMetaType>

struct Flight
{
    QString id;
    double latitude;
    double longitude;
    double altitude;
    double speed;
    double heading;
    bool inConflict = false;
};

Q_DECLARE_METATYPE(Flight)
Q_DECLARE_METATYPE(std::vector<Flight>)
