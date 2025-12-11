#include "datagenerator.h"
#include "conflictdetector.h"

#include <QRandomGenerator>
#include <QtMath>

DataGenerator::DataGenerator(QObject *parent)
    : QObject(parent)
    , m_timer(this)
{
    m_timer.setInterval(m_intervalMs);
    connect(&m_timer, &QTimer::timeout,
            this, &DataGenerator::onTick);
}

void DataGenerator::setParameters(int numFlights, double conflictThreshold, int intervalMs)
{
    m_numFlights = qMax(1, numFlights);
    m_conflictThreshold = conflictThreshold;
    m_intervalMs = intervalMs;

    m_timer.setInterval(m_intervalMs);

    if (!m_flights.empty()) {
        initFlights();
        emit flightsUpdated(m_flights);
    }
}

void DataGenerator::initFlights()
{
    m_flights.clear();

    auto *rng = QRandomGenerator::global();

    for (int i = 0; i < m_numFlights; ++i) {
        Flight f;
        f.id        = QStringLiteral("AC%1").arg(i + 1, 3, 10, QLatin1Char('0'));
        f.latitude  = 46.0 + rng->bounded(2.0);      // 46 .. 48
        f.longitude = 25.0 + rng->bounded(3.0);      // 25 .. 28
        f.altitude  = 9000 + rng->bounded(4000.0);   // 9000 .. 13000
        f.speed     = 400 + rng->bounded(100.0);     // 400 .. 500
        f.heading   = rng->bounded(360.0);           // 0 .. 359
        f.inConflict = false;

        m_flights.push_back(f);
    }
}

void DataGenerator::start()
{
    if (m_timer.isActive())
        return;

    if (m_flights.empty())
        initFlights();

    m_timer.start();
}

void DataGenerator::stop()
{
    m_timer.stop();
}

void DataGenerator::onTick()
{
    const double dt = 1.0;
    const double scale = 0.0002;

    for (Flight &f : m_flights) {
        double distance = f.speed * dt * scale;
        double rad = qDegreesToRadians(f.heading);
        f.latitude  += qSin(rad) * distance;
        f.longitude += qCos(rad) * distance;
    }

    for (Flight &f : m_flights)
        f.inConflict = false;

    auto conflicts = ConflictDetector::detectConflicts(m_flights, m_conflictThreshold);

    for (const auto &pair : conflicts) {
        int i = pair.first;
        int j = pair.second;
        if (i >= 0 && i < static_cast<int>(m_flights.size()))
            m_flights[i].inConflict = true;
        if (j >= 0 && j < static_cast<int>(m_flights.size()))
            m_flights[j].inConflict = true;
    }

    emit flightsUpdated(m_flights);
}
