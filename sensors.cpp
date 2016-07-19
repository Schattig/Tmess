#include "sensors.h"

sensors::sensors(QObject *parent) : QObject(parent)
{
    sensorList = new QStringList();
}

void sensors::appendSensor(QString serial)
{
    sensorList->append(serial);
}

QStringList sensors::getList()
{
    return *sensorList;
}
