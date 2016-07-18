#ifndef SENSORBOX_H
#define SENSORBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QList>

#include "sensors.h"

class SensorBox : public QWidget
{
    Q_OBJECT
public:
    enum Position { Front, Back };

    struct sensorEntry{
        QString name;
        sensors::Port port;
    };

    explicit SensorBox(QString name, QWidget *parent = 0);

    void setEnabled(bool enable);
    void writeTemp(int temp, int sensor, Position position);

    void addPort(QString text, sensors::Port port);
    void addPort(QList<sensorEntry> sensors);

    bool isEnabled();

    sensors::Port getPort();

signals:
    void checkChanged(bool state);
    void portChanged(sensors::Port);

private slots:
    void checkActiveChanged(int state);
    int portComboboxChanged(int index);

private:
    enum {NumSensorB = 3, NumSensorF = 9};
    QLineEdit *sensorsF[NumSensorF];
    QLineEdit *sensorsB[NumSensorB];
    QComboBox *pinSelect;
    bool enabled = false;

    sensors::Port currentPort;

    void clearTemp();
};

#endif // SENSORBOX_H
