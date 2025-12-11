#pragma once

#include <QAbstractTableModel>
#include <vector>
#include "flight.h"

class FlightsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FlightsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void setFlights(const std::vector<Flight> &flights);
    const std::vector<Flight> &flights() const { return m_flights; }

private:
    std::vector<Flight> m_flights;
};
