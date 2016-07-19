#include "sensors.h"

sensors::sensors(QObject *parent) : QObject(parent)
{
    sensorList = new QStringList();
}

sensors::sensors(const sensors &s) : QObject()
{
    this->sensorList = s.sensorList;
}

void sensors::insertSensor(int i, QString serial)
{
    sensorList->insert(i, serial);
}

void sensors::replaceSensor(int i ,QString serial)
{
    sensorList->replace(i, serial);
}

QStringList sensors::getList()
{
    return *sensorList;
}

int sensors::length()
{
    return sensorList->length();
}

QString sensors::sensorAt(int index)
{
    return sensorList->at(index);
}
