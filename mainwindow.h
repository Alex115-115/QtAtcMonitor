#pragma once

#include <QMainWindow>
#include <QThread>
#include <vector>

#include "flightsmodel.h"
#include "appconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class DataGenerator;
class QTextEdit;
class FlightsProxyModel;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFlightsUpdated(const std::vector<Flight> &flights);
    void startSimulation();
    void stopSimulation();
    void onShowConflictsToggled(bool checked);
    void openSettings();
    void exportLogToFile();

private:
    void setupMenusAndToolbars();
    void updateStatusBar();
    void logMessage(const QString &text);
    void applyConfigToGenerator();

    Ui::MainWindow *ui;
    FlightsModel *m_flightsModel;
    FlightsProxyModel *m_proxyModel = nullptr;

    QThread *m_workerThread = nullptr;
    DataGenerator *m_dataGenerator = nullptr;

    QTextEdit *m_logView = nullptr;
    std::vector<Flight> m_lastFlights;

    QAction *m_showConflictsAction = nullptr;
    QAction *m_startAction = nullptr;
    QAction *m_stopAction = nullptr;
    QAction *m_exportLogAction = nullptr;
    QAction *m_settingsAction = nullptr;

    AppConfig m_config;
};
