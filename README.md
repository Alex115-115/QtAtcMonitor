# ATC Monitor – Air Traffic Simulation Dashboard (Qt + C++17)

ATC Monitor is a desktop application built with Qt 6 and C++17 that simulates a simplified air traffic monitoring system:

- 2D radar display (custom QWidget) with moving aircraft
- Detailed flights table with sorting and conflict filtering
- Conflict detection between aircraft based on configurable distance threshold
- Background simulation in a worker thread
- Event log for conflicts and simulation lifecycle
- Simple settings dialog + persistent configuration (.ini file)
- Basic unit tests for the conflict detection logic

## Features

- **Radar View**
  - Custom `QWidget` (`RadarView`) painted with `QPainter`
  - Normalized coordinates based on latitude/longitude
  - Aircraft in conflict highlighted in red

- **Flights Table**
  - `QAbstractTableModel` (`FlightsModel`) holding `Flight` structs
  - Columns: ID, Latitude, Longitude, Altitude, Speed, Heading, Status (OK/CONFLICT)
  - `QSortFilterProxyModel` (`FlightsProxyModel`) for:
    - column sorting
    - "Show conflicts only" filtering

- **Simulation**
  - `DataGenerator` runs in a separate `QThread`
  - Internal `QTimer` ticks at a configurable interval
  - Generates and updates a set of flights with random positions and headings
  - Emits `flightsUpdated(std::vector<Flight>)` to update the UI

- **Conflict Detection**
  - `ConflictDetector` – pure C++ module
  - Checks pairwise distances between flights and flags conflicts under a threshold
  - Threshold is configurable from the settings dialog

- **Logging**
  - `QTextEdit` used as an event log
  - Events logged:
    - Simulation start/stop
    - New flight entering the radar area
    - Flight leaving the radar area (if implemented)
    - Conflict detected / resolved

- **Settings & Persistence**
  - `SettingsDialog` to configure:
    - Number of flights
    - Conflict distance threshold
    - Simulation update interval (ms)
  - `AppConfig` + `AppConfigManager` use `QSettings` with an `.ini` file
    - `atcmonitor.ini` stored next to the executable

- **Tests**
  - Qt Test–based unit tests in `tests/`
  - `ConflictDetectorTests` covers basic scenarios:
    - no conflict
    - single conflict

## Build Instructions

### Requirements

- CMake >= 3.16
- Qt 6 (or Qt 5) with Widgets and Test modules
- A C++17–capable compiler

### Configure & build

```bash
mkdir build
cd build
cmake ..
cmake --build .
