#pragma once

#include <QObject>
#include <QTimer>
#include <vector>
#include "flight.h"

class DataGenerator : public QObject
{
    Q_OBJECT
public:
    explicit DataGenerator(QObject *parent = nullptr);

public slots:
    void start();
    void stop();

    void setParameters(int numFlights, double conflictThreshold, int intervalMs);

signals:
    void flightsUpdated(const std::vector<Flight> &flights);

private slots:
    void onTick();

private:
    QTimer m_timer;
    std::vector<Flight> m_flights;

    int m_numFlights = 8;
    double m_conflictThreshold = 0.15;
    int m_intervalMs = 200;

    void initFlights();
};
