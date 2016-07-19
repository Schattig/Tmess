#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QAction>
#include "settingsdialog.h"
#include "uart.h"
#include "console.h"
#include "COM_handler.h"
#include "sensorbox.h"
#include "modbus.h"
#include "pyranobox.h"
#include "mod_handler.h"
//#include "sensors.h"
#include "sensordialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void serialOpened();
    void serialClosed();

    void modOpened();
    void modClosed();

    void messStart();
    void messStop();

    void monitorCheckChanged(bool state);

    void modUpdate();

    void test();

private:
    void createActions();
    void createStatusBar();
    void createConnections();
    void createComboEntrys();

    SettingsDialog *settings;
    SensorDialog *sensorSettings;
    UART *uart;
    Console *console;
    ModBus *modbus;

    SensorBox *monitor1;
    SensorBox *monitor2;
    PyranoBox *pyranoBox;

    COM_handler *comh;
    MOD_handler *modh;

    QAction *actionSettings;
    QAction *actionSensorSettings;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionClear;

    QAction *actionStart;
    QAction *actionStop;

    QAction *actionTest;

    QTimer *messIntervall;
};

#endif // MAINWINDOW_H
