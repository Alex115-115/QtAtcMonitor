#pragma once

#include <vector>
#include <utility>
#include "flight.h"

namespace ConflictDetector {

std::vector<std::pair<int, int>> detectConflicts(const std::vector<Flight> &flights,
                                                 double distanceThreshold);

} // namespace ConflictDetector
