#ifndef MOD_HANDLER_H
#define MOD_HANDLER_H

#include <QObject>

#include <QModbusDataUnit>
#include "modbus.h"

class MOD_handler : public QObject
{
    Q_OBJECT
public:
    explicit MOD_handler(ModBus *mod, QObject *parent = 0);
    int getTemp();
    int getWatt();

signals:
    void update();

public slots:
    void putData(QModbusDataUnit data);

private:
    int temp;
    int watt;

    ModBus *mod;

};

#endif // MOD_HANDLER_H
