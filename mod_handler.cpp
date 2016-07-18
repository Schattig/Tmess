#include "mod_handler.h"
#include <QDebug>

MOD_handler::MOD_handler(ModBus *mod, QObject *parent) : QObject(parent)
{
    this->mod = mod;

    connect(mod, &ModBus::readyRead, this, &MOD_handler::putData);
}

void MOD_handler::putData(QModbusDataUnit data)
{
    for(uint i = 0; i < data.valueCount(); i++)
    {
        if(i == 8)
            temp = static_cast<int>(data.value(static_cast<int>(i)));
        if(i == 5)
            watt = static_cast<int>(data.value(static_cast<int>(i)));
    }

    //qDebug() << "temp: " << temp;
    //qDebug() << "watt: " << watt;

    emit update();
}

int MOD_handler::getTemp()
{
    return temp;
}

int MOD_handler::getWatt()
{
    return watt;
}
