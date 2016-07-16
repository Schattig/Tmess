#ifndef MODBUS_H
#define MODBUS_H

#include <QObject>

#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>

#include "settingsdialog.h"

QT_BEGIN_NAMESPACE

class QModbusClient;
class QModbusReply;

QT_END_NAMESPACE

class ModBus : public QObject
{
    Q_OBJECT

signals:
    void readyRead(QModbusDataUnit data);
    void isOpened();
    void isClosed();

public:
    explicit ModBus(SettingsDialog::ModBusSettings s, QObject *parent = 0);

    void setQuery(int startAdress, int numberOfEntries);

public slots:
    void openModBusPort();
    void closeModBusPort();
    void readData();
    void changeSettings(SettingsDialog::ModBusSettings s);

private slots:
    void readFinished();

private:
    SettingsDialog::ModBusSettings settings;
    //QModbusReply *lastRequest;
    QModbusClient *modbusDevice;


};

#endif // MODBUS_H
