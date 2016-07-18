#ifndef SENSORBOX_H
#define SENSORBOX_H

#include <QWidget>
#include <QLineEdit>

class SensorBox : public QWidget
{
    Q_OBJECT
public:
    enum Position { Front, Back };

    explicit SensorBox(QString name, QWidget *parent = 0);

    void setEnabled(bool enable);
    void writeTemp(int temp, int sensor, Position position);

    bool isEnabled();

signals:
    void checkChanged(bool state);

public slots:
    void checkActiveChanged(int state);

private:
    enum {NumSensorB = 3, NumSensorF = 9};
    QLineEdit *sensorsF[NumSensorF];
    QLineEdit *sensorsB[NumSensorB];
    bool enabled = false;

    void clearTemp();
};

#endif // SENSORBOX_H
