#include <QtTest>
#include "conflictdetector.h"
#include "flight.h"

class ConflictDetectorTests : public QObject
{
    Q_OBJECT
private slots:
    void noConflicts();
    void singleConflict();
};

void ConflictDetectorTests::noConflicts()
{
    std::vector<Flight> flights{
        { "A", 0.0, 0.0, 10000, 400, 90, false },
        { "B", 10.0, 10.0, 10000, 400, 90, false },
    };

    auto conflicts = ConflictDetector::detectConflicts(flights, 1.0);
    QCOMPARE(conflicts.size(), std::size_t(0));
}

void ConflictDetectorTests::singleConflict()
{
    std::vector<Flight> flights{
        { "A", 0.0, 0.0, 10000, 400, 90, false },
        { "B", 0.1, 0.1, 10000, 400, 90, false },
    };

    auto conflicts = ConflictDetector::detectConflicts(flights, 0.5);
    QCOMPARE(conflicts.size(), std::size_t(1));
    QCOMPARE(conflicts[0].first, 0);
    QCOMPARE(conflicts[0].second, 1);
}

QTEST_MAIN(ConflictDetectorTests)
#include "test_conflict_detector.moc"
