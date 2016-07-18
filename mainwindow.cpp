#include "mainwindow.h"

#include <QWidget>
#include <QMessageBox>
#include <QByteArray>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include "settingsdialog.h"
#include "console.h"
#include "borderlayout.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setMinimumHeight(400);
    setMinimumWidth(700);

    QWidget *centralWidget = new QWidget(this);
    QWidget *eastWidget = new QWidget(this);
    BorderLayout *mainLayout = new BorderLayout();
    BorderLayout *eastLayout = new BorderLayout();

    console = new Console(this);
    settings = new SettingsDialog(this);
    uart = new UART(settings->settings());
    modbus = new ModBus(settings->modSettings());
    comh = new COM_handler(uart);
    modh = new MOD_handler(modbus);

    messIntervall = new QTimer(this);
    messIntervall->setInterval(3000);

    monitor1 = new SensorBox("Monitor 1", this);
    monitor2 = new SensorBox("Monitor 2", this);
    pyranoBox = new PyranoBox("Pyrano Meter", this);

    mainLayout->addWidget(console, BorderLayout::Center);
    mainLayout->addWidget(eastWidget, BorderLayout::East);
    eastLayout->addWidget(monitor2, BorderLayout::East);
    eastLayout->addWidget(monitor1, BorderLayout::East);
    eastLayout->addWidget(pyranoBox, BorderLayout::North);

    eastWidget->setLayout(eastLayout);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    createActions();
    createStatusBar();
    createConnections();
}
MainWindow::~MainWindow()

{
    delete uart;
    delete modbus;
    delete comh;
    delete modh;
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

    fileToolBar->addSeparator();
    fileToolBar->addSeparator();

    actionReadModBus = new QAction(tr("read mod"), this);
    actionReadModBus->setDisabled(true);
    connect(actionReadModBus, SIGNAL( triggered() ), modbus, SLOT( readData() ));
    fileToolBar->addAction(actionReadModBus);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createConnections()
{
    connect(settings, SIGNAL(changed(SettingsDialog::Settings)),
            uart, SLOT(changeSettings(SettingsDialog::Settings)) );

    connect(settings, SIGNAL(modChanged(SettingsDialog::ModBusSettings)),
            modbus, SLOT(changeSettings(SettingsDialog::ModBusSettings)) );

    connect(uart, SIGNAL(isOpened()), this, SLOT(serialOpened()));
    connect(uart, SIGNAL(isClosed()), this, SLOT(serialClosed()));

    connect(modbus, SIGNAL(isOpened()), this, SLOT(modOpened()));
    connect(modbus, SIGNAL(isClosed()), this, SLOT(modClosed()));

    connect(console, &Console::getData, uart, &UART::write);

    connect(messIntervall, SIGNAL( timeout() ), comh, SLOT( messung() ));
    connect(messIntervall, SIGNAL( timeout() ), modbus, SLOT(readData() ));

    connect(monitor1, SIGNAL(checkChanged(bool)), this, SLOT(monitorCheckChanged(bool)));
    connect(monitor2, SIGNAL(checkChanged(bool)), this, SLOT(monitorCheckChanged(bool)));

    connect(modh, SIGNAL(update()), this, SLOT(modUpdate()));


    /* DEBUG Ausgabe -> alles auf konsole ausgeben */
    connect(uart, &UART::readyRead, console, &Console::putData);
}

void MainWindow::serialOpened()
{
    console->setLocalEchoEnabled(settings->getEchoState());
    console->setEnabled(true);
    actionConnect->setDisabled(true);
    actionDisconnect->setEnabled(true);
    actionStart->setEnabled(true);
    comh->connected();

    if(monitor1->isEnabled())
        monitor1->setEnabled(true);

    if(monitor2->isEnabled())
        monitor2->setEnabled(true);
}

void MainWindow::serialClosed()
{
    messIntervall->stop();
    console->setDisabled(true);
    actionConnect->setEnabled(true);
    actionDisconnect->setDisabled(true);
    actionStart->setDisabled(true);
    actionStop->setDisabled(true);

    monitor1->setEnabled(false);
    monitor2->setEnabled(false);
}

void MainWindow::modOpened()
{
    actionReadModBus->setEnabled(true);

    pyranoBox->clear();
    pyranoBox->setEnabled(true);
}

void MainWindow::modClosed()
{
    actionReadModBus->setDisabled(true);

    pyranoBox->clear();
    pyranoBox->setEnabled(false);
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

void MainWindow::monitorCheckChanged(bool state)
{
    if(!uart->isOpen())
        return;

    if(QObject::sender() == monitor1)
        monitor1->setEnabled(state);

    if(QObject::sender() == monitor2)
        monitor2->setEnabled(state);
}

void MainWindow::modUpdate()
{
    pyranoBox->writeTemp(modh->getTemp());
    pyranoBox->writeWatt(modh->getWatt());
}
