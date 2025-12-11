#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "radarview.h"
#include "datagenerator.h"
#include "flightsproxymodel.h"
#include "settingsdialog.h"
#include "appconfig.h"

#include <QHeaderView>
#include <QMenuBar>
#include <QStatusBar>
#include <QAction>
#include <QTextEdit>
#include <QTime>
#include <QHash>
#include <QSet>
#include <QToolBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QMetaObject>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_flightsModel(new FlightsModel(this))
{
    ui->setupUi(this);

    m_config = AppConfigManager::load();

    m_proxyModel = new FlightsProxyModel(this);
    m_proxyModel->setSourceModel(m_flightsModel);

    ui->tableViewFlights->setModel(m_proxyModel);
    ui->tableViewFlights->setSortingEnabled(true);
    ui->tableViewFlights->horizontalHeader()
        ->setSectionResizeMode(QHeaderView::Stretch);

    ui->radarView->setModel(m_flightsModel);

    m_logView = new QTextEdit(this);
    m_logView->setObjectName("logView");
    m_logView->setReadOnly(true);
    ui->verticalLayout->addWidget(m_logView);

    setupMenusAndToolbars();
    updateStatusBar();

    m_workerThread = new QThread(this);
    m_dataGenerator = new DataGenerator();

    m_dataGenerator->moveToThread(m_workerThread);

    connect(m_dataGenerator, &DataGenerator::flightsUpdated,
            this, &MainWindow::onFlightsUpdated,
            Qt::QueuedConnection);

    m_workerThread->start();

    applyConfigToGenerator();

    logMessage(tr("Loaded settings: flights=%1, threshold=%2, interval=%3 ms")
                   .arg(m_config.numFlights)
                   .arg(m_config.conflictThreshold)
                   .arg(m_config.tickIntervalMs));

}

MainWindow::~MainWindow()
{
    if (m_workerThread && m_workerThread->isRunning() && m_dataGenerator) {
        QMetaObject::invokeMethod(m_dataGenerator, "stop", Qt::QueuedConnection);
        m_workerThread->quit();
        m_workerThread->wait();
    }

    delete m_dataGenerator;
    AppConfigManager::save(m_config);
    delete ui;
}


void MainWindow::setupMenusAndToolbars()
{
    // ----- File -----
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    m_exportLogAction = fileMenu->addAction(tr("Export &Log..."));
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));

    connect(m_exportLogAction, &QAction::triggered,
            this, &MainWindow::exportLogToFile);
    connect(exitAction, &QAction::triggered,
            this, &QWidget::close);

    // ----- Simulation -----
    QMenu *simMenu = menuBar()->addMenu(tr("&Simulation"));
    m_startAction = simMenu->addAction(tr("&Start"));
    m_stopAction  = simMenu->addAction(tr("S&top"));
    simMenu->addSeparator();
    m_settingsAction = simMenu->addAction(tr("&Settings..."));

    connect(m_startAction, &QAction::triggered,
            this, &MainWindow::startSimulation);
    connect(m_stopAction, &QAction::triggered,
            this, &MainWindow::stopSimulation);
    connect(m_settingsAction, &QAction::triggered,
            this, &MainWindow::openSettings);

    // ----- View -----
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    m_showConflictsAction =
        viewMenu->addAction(tr("Show &conflicts only"));
    m_showConflictsAction->setCheckable(true);
    connect(m_showConflictsAction, &QAction::toggled,
            this, &MainWindow::onShowConflictsToggled);

    // ----- Toolbar -----
    QToolBar *toolbar = addToolBar(tr("Simulation"));
    toolbar->addAction(m_startAction);
    toolbar->addAction(m_stopAction);
    toolbar->addSeparator();
    toolbar->addAction(m_showConflictsAction);
}

void MainWindow::updateStatusBar()
{
    int count = m_flightsModel->rowCount();
    statusBar()->showMessage(tr("Active flights (total): %1").arg(count));
}

void MainWindow::logMessage(const QString &text)
{
    if (!m_logView)
        return;

    const QString time = QTime::currentTime().toString("HH:mm:ss");
    m_logView->append(QStringLiteral("[%1] %2").arg(time, text));
}

void MainWindow::applyConfigToGenerator()
{
    if (!m_dataGenerator || !m_workerThread || !m_workerThread->isRunning())
        return;

    QMetaObject::invokeMethod(
        m_dataGenerator, "setParameters", Qt::QueuedConnection,
        Q_ARG(int, m_config.numFlights),
        Q_ARG(double, m_config.conflictThreshold),
        Q_ARG(int, m_config.tickIntervalMs));
}


void MainWindow::onFlightsUpdated(const std::vector<Flight> &flights)
{
    QHash<QString, bool> prevConflict;
    for (const Flight &f : m_lastFlights)
        prevConflict.insert(f.id, f.inConflict);

    QHash<QString, bool> currConflict;
    for (const Flight &f : flights)
        currConflict.insert(f.id, f.inConflict);

    QSet<QString> prevIds = QSet<QString>(prevConflict.keyBegin(),
                                          prevConflict.keyEnd());
    QSet<QString> currIds = QSet<QString>(currConflict.keyBegin(),
                                          currConflict.keyEnd());

    for (const QString &id : currIds - prevIds)
        logMessage(tr("Flight %1 entered radar area").arg(id));

    for (const QString &id : prevIds - currIds)
        logMessage(tr("Flight %1 left radar area").arg(id));

    for (const QString &id : currIds) {
        bool was = prevConflict.value(id, false);
        bool now = currConflict.value(id, false);

        if (!was && now)
            logMessage(tr("Conflict detected for flight %1").arg(id));
        else if (was && !now)
            logMessage(tr("Conflict resolved for flight %1").arg(id));
    }

    m_flightsModel->setFlights(flights);
    m_lastFlights = flights;
    updateStatusBar();
}

void MainWindow::startSimulation()
{
    if (!m_workerThread || !m_workerThread->isRunning() || !m_dataGenerator)
        return;

    applyConfigToGenerator();
    logMessage(tr("Starting simulation"));

    QMetaObject::invokeMethod(m_dataGenerator, "start", Qt::QueuedConnection);
}


void MainWindow::stopSimulation()
{
    if (!m_workerThread || !m_workerThread->isRunning() || !m_dataGenerator)
        return;

    logMessage(tr("Stopping simulation"));
    QMetaObject::invokeMethod(m_dataGenerator, "stop", Qt::QueuedConnection);
    logMessage(tr("Simulation stopped"));
}


void MainWindow::onShowConflictsToggled(bool checked)
{
    if (m_proxyModel)
        m_proxyModel->setOnlyConflicts(checked);
}

void MainWindow::openSettings()
{
    SettingsDialog dlg(this);
    dlg.setConfig(m_config);

    if (dlg.exec() == QDialog::Accepted) {
        m_config = dlg.config();
        AppConfigManager::save(m_config);
        applyConfigToGenerator();
        logMessage(tr("Simulation settings updated"));
    }
}

void MainWindow::exportLogToFile()
{
    if (!m_logView)
        return;

    const QString fileName =
        QFileDialog::getSaveFileName(this,
                                     tr("Export Log"),
                                     QString(),
                                     tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this,
                             tr("Export Log"),
                             tr("Cannot open file for writing:\n%1")
                                 .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out << m_logView->toPlainText();

    logMessage(tr("Log exported to %1").arg(fileName));
}
