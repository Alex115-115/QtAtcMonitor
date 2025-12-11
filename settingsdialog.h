#pragma once

#include <QDialog>
#include "appconfig.h"

class QSpinBox;
class QDoubleSpinBox;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);

    void setConfig(const AppConfig &cfg);
    AppConfig config() const;

private:
    QSpinBox *m_numFlightsSpin = nullptr;
    QDoubleSpinBox *m_conflictThresholdSpin = nullptr;
    QSpinBox *m_tickIntervalSpin = nullptr;
};
