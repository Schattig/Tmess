#ifndef COM_handler_H
#define COM_handler_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QStateMachine>
#include <QList>
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
    //QList<sensor*> *allSensors;
    QList<sensor*> *mon1F;
    QList<sensor*> *mon1B;
    QList<sensor*> *mon2F;
    QList<sensor*> *mon2B;
    QList<sensor*> *other;
    QByteArray *log;

    bool idStart = false;
    bool idMeas = false;
    bool reqSerial = false;
    bool sendAll = false;

    bool mon1 = true;
    bool mon2 = true;
    bool others = true;

    QByteArray *readID;
    QByteArray *sendID;
    bool lastCRC = false;
    QString lastTemp;

    void sendAllSensors();
    void sendSensor(sensor *s);

private slots:
    void updateAllSensors();

};

#endif // COM_handler_H
