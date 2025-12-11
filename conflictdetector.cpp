#include "conflictdetector.h"
#include <QtMath>

namespace ConflictDetector {

std::vector<std::pair<int, int>> detectConflicts(const std::vector<Flight> &flights,
                                                 double distanceThreshold)
{
    std::vector<std::pair<int, int>> result;
    const int n = static_cast<int>(flights.size());
    if (n < 2)
        return result;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = flights[i].longitude - flights[j].longitude;
            double dy = flights[i].latitude  - flights[j].latitude;
            double dist = qSqrt(dx * dx + dy * dy);
            if (dist < distanceThreshold) {
                result.emplace_back(i, j);
            }
        }
    }

    return result;
}

} // namespace ConflictDetector
