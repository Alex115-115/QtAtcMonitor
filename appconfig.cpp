#include "appconfig.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDir>

AppConfig AppConfigManager::load()
{
    AppConfig cfg;

    const QString iniPath =
        QCoreApplication::applicationDirPath() + QDir::separator() + "atcmonitor.ini";

    QSettings settings(iniPath, QSettings::IniFormat);

    cfg.numFlights = settings.value("simulation/numFlights", cfg.numFlights).toInt();
    cfg.conflictThreshold =
        settings.value("simulation/conflictThreshold", cfg.conflictThreshold).toDouble();
    cfg.tickIntervalMs =
        settings.value("simulation/tickIntervalMs", cfg.tickIntervalMs).toInt();

    return cfg;
}

void AppConfigManager::save(const AppConfig &cfg)
{
    const QString iniPath =
        QCoreApplication::applicationDirPath() + QDir::separator() + "atcmonitor.ini";

    QSettings settings(iniPath, QSettings::IniFormat);

    settings.setValue("simulation/numFlights", cfg.numFlights);
    settings.setValue("simulation/conflictThreshold", cfg.conflictThreshold);
    settings.setValue("simulation/tickIntervalMs", cfg.tickIntervalMs);
}
