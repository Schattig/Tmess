#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include "settingsdialog.h"
#include "uart.h"
#include "console.h"
#include "COM_handler.h"
#include "sensorbox.h"
#include "modbus.h"


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

    void messStart();
    void messStop();

private:
    void createActions();
    void createStatusBar();

    SettingsDialog *settings;
    UART *uart;
    Console *console;
    ModBus *modbus;

    SensorBox *monitor1;
    SensorBox *monitor2;

    COM_handler *smh;

    QAction *actionSettings;
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionClear;

    QAction *actionStart;
    QAction *actionStop;

    QTimer *messIntervall;

};

#endif // MAINWINDOW_H
