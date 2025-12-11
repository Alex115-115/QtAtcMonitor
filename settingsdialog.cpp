#include "settingsdialog.h"

#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Simulation Settings"));

    auto *layout = new QFormLayout(this);

    m_numFlightsSpin = new QSpinBox(this);
    m_numFlightsSpin->setRange(1, 200);
    m_numFlightsSpin->setValue(8);

    m_conflictThresholdSpin = new QDoubleSpinBox(this);
    m_conflictThresholdSpin->setDecimals(3);
    m_conflictThresholdSpin->setSingleStep(0.01);
    m_conflictThresholdSpin->setRange(0.01, 5.0);
    m_conflictThresholdSpin->setValue(0.15);

    m_tickIntervalSpin = new QSpinBox(this);
    m_tickIntervalSpin->setRange(50, 2000);
    m_tickIntervalSpin->setSingleStep(50);
    m_tickIntervalSpin->setValue(200);

    layout->addRow(tr("Number of flights:"), m_numFlightsSpin);
    layout->addRow(tr("Conflict threshold:"), m_conflictThresholdSpin);
    layout->addRow(tr("Update interval (ms):"), m_tickIntervalSpin);

    auto *buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addRow(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted,
            this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            this, &QDialog::reject);
}

void SettingsDialog::setConfig(const AppConfig &cfg)
{
    m_numFlightsSpin->setValue(cfg.numFlights);
    m_conflictThresholdSpin->setValue(cfg.conflictThreshold);
    m_tickIntervalSpin->setValue(cfg.tickIntervalMs);
}

AppConfig SettingsDialog::config() const
{
    AppConfig cfg;
    cfg.numFlights = m_numFlightsSpin->value();
    cfg.conflictThreshold = m_conflictThresholdSpin->value();
    cfg.tickIntervalMs = m_tickIntervalSpin->value();
    return cfg;
}
