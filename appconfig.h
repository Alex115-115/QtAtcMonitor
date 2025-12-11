#pragma once

struct AppConfig
{
    int numFlights = 8;
    double conflictThreshold = 0.15;
    int tickIntervalMs = 200;
};

class AppConfigManager
{
public:
    static AppConfig load();
    static void save(const AppConfig &cfg);
};
