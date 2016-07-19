#ifndef COM_handler_H
#define COM_handler_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStateMachine>
#include "uart.h"
#include "sensordialog.h"

class COM_handler : public QObject
{
    Q_OBJECT

signals:
    //void id_idle_wait();
    //void id_wait_rdyread();
    void serialReady(QString serial);
    void measReady(QString temp);

public:
    COM_handler(UART *uart, SensorDialog *sDialog);

public slots:
    void putData(const QByteArray &data);
    void connected();
    void messung();
    void requestSerial();

private:
    UART *uart;
    SensorDialog *sDialog;
    SensorDialog::monitor mon1;
    SensorDialog::monitor mon2;
    sensors *other;
    QByteArray *log;

    bool idStart = false;
    bool idMeas = false;
    bool reqSerial = false;
    QByteArray *readID;
    QByteArray *sendID;
    bool lastCRC = false;
    QString lastTemp;
};

#endif // COM_handler_H
