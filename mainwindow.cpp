#include "mainwindow.h"

#include <QWidget>
#include <QMessageBox>
#include <QByteArray>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include "settingsdialog.h"
#include "console.h"
#include "borderlayout.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumHeight(300);
    setMinimumWidth(700);

    QWidget *centralWidget = new QWidget(this);
    QWidget *eastWidget = new QWidget(this);
    BorderLayout *mainLayout = new BorderLayout;
    BorderLayout *eastLayout = new BorderLayout;

    console = new Console;
    settings = new SettingsDialog;
    uart = new UART(settings->settings());
    modbus = new ModBus(settings->modSettings());
    smh = new COM_handler(uart);

    messIntervall = new QTimer(this);
    messIntervall->setInterval(3000);

    monitor1 = new SensorBox("Monitor 1");
    monitor2 = new SensorBox("Monitor 2");

    mainLayout->addWidget(console, BorderLayout::Center);
    mainLayout->addWidget(eastWidget, BorderLayout::East);
    eastLayout->addWidget(monitor2, BorderLayout::East);
    eastLayout->addWidget(monitor1, BorderLayout::East);

    eastWidget->setLayout(eastLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    createActions();
    createStatusBar();

    connect(settings, SIGNAL(changed(SettingsDialog::Settings)),
            uart, SLOT(changeSettings(SettingsDialog::Settings)) );

    connect(settings, SIGNAL(modChanged(SettingsDialog::ModBusSettings)),
            modbus, SLOT(changeSettings(SettingsDialog::ModBusSettings)) );

    connect(uart, SIGNAL(isOpened()), this, SLOT(serialOpened()));
    connect(uart, SIGNAL(isClosed()), this, SLOT(serialClosed()));

    connect(uart, &UART::readyRead, smh, &COM_handler::putData);

    connect(console, &Console::getData, uart, &UART::write);

    connect(messIntervall, SIGNAL( timeout() ), smh, SLOT( messung() ));

    /* DEBUG Ausgabe -> alles auf konsole ausgeben */
    connect(uart, &UART::readyRead, console, &Console::putData);
}
MainWindow::~MainWindow()

{

}

void MainWindow::serialOpened()
{
    console->setLocalEchoEnabled(settings->getEchoState());
    console->setEnabled(true);
    actionConnect->setDisabled(true);
    actionDisconnect->setEnabled(true);
    actionStart->setEnabled(true);
    smh->connected();
}

void MainWindow::serialClosed()
{
    messIntervall->stop();
    console->setDisabled(true);
    actionConnect->setEnabled(true);
    actionDisconnect->setDisabled(true);
    actionStart->setDisabled(true);
    actionStop->setDisabled(true);
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QToolBar *fileToolBar = addToolBar(tr("File"));

    actionSettings = new QAction(tr("Settings"), this);
    connect(actionSettings, &QAction::triggered, settings, &MainWindow::show);
    fileMenu->addAction(actionSettings);
    fileToolBar->addAction(actionSettings);

    fileMenu->addSeparator();
    fileToolBar->addSeparator();

    actionConnect = new QAction(tr("connect"), this);
    connect(actionConnect, &QAction::triggered, uart, &UART::openSerialPort);
    connect(actionConnect, &QAction::triggered, modbus, &ModBus::openModBusPort);
    fileMenu->addAction(actionConnect);
    fileToolBar->addAction(actionConnect);

    actionDisconnect = new QAction(tr("Disconnect"), this);
    actionDisconnect->setDisabled(true);
    connect(actionDisconnect, &QAction::triggered, uart, &UART::closeSerialPort);
    connect(actionDisconnect, &QAction::triggered, modbus, &ModBus::closeModBusPort);
    fileMenu->addAction(actionDisconnect);
    fileToolBar->addAction(actionDisconnect);

    fileToolBar->addSeparator();

    actionClear = new QAction(tr("clear"), this);
    connect(actionClear, &QAction::triggered, console, &Console::clear);
    fileToolBar->addAction(actionClear);

    fileToolBar->addSeparator();
    fileToolBar->addSeparator();

    actionStart = new QAction(tr("start"), this);
    actionStart->setDisabled(true);
    connect(actionStart, SIGNAL( triggered() ), this, SLOT( messStart() ));
    fileToolBar->addAction(actionStart);

    actionStop = new QAction(tr("stop"), this);
    actionStop->setDisabled(true);
    connect(actionStop, SIGNAL( triggered() ), this, SLOT( messStop() ));
    fileToolBar->addAction(actionStop);

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::messStart()
{
    actionStart->setDisabled(true);
    actionStop->setEnabled(true);
    messIntervall->start();
}

void MainWindow::messStop()
{
    messIntervall->stop();
    actionStop->setDisabled(true);
    actionStart->setEnabled(true);
}
