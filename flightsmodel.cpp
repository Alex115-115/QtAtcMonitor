#include "flightsmodel.h"
#include <QString>

FlightsModel::FlightsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_flights = {
        { "RO001", 47.15, 27.60, 10000, 450,  90, false },
        { "LH123", 46.80, 25.00, 12000, 480, 135, false },
        { "AF789", 45.90, 26.30,  9000, 430, 270, true  },
        { "WZ555", 48.10, 28.10, 11000, 460,  45, false }
    };
}

int FlightsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return static_cast<int>(m_flights.size());
}

int FlightsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    // id, lat, lon, alt, speed, heading, status
    return 7;
}

QVariant FlightsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    if (role != Qt::DisplayRole)
        return {};

    const Flight &f = m_flights.at(index.row());

    switch (index.column()) {
    case 0: return f.id;
    case 1: return f.latitude;
    case 2: return f.longitude;
    case 3: return f.altitude;
    case 4: return f.speed;
    case 5: return f.heading;
    case 6: return f.inConflict ? QStringLiteral("CONFLICT") : QStringLiteral("OK");
    default: return {};
    }
}

QVariant FlightsModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0: return QStringLiteral("ID");
        case 1: return QStringLiteral("Lat");
        case 2: return QStringLiteral("Lon");
        case 3: return QStringLiteral("Alt");
        case 4: return QStringLiteral("Speed");
        case 5: return QStringLiteral("Heading");
        case 6: return QStringLiteral("Status");
        default: return {};
        }
    }

    return section + 1;
}

void FlightsModel::setFlights(const std::vector<Flight> &flights)
{
    beginResetModel();
    m_flights = flights;
    endResetModel();
}
