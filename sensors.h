#ifndef SENSORS_H
#define SENSORS_H

#include <QObject>
#include <QStringList>

class sensors : public QObject
{
    Q_OBJECT
public:

    enum Port{ Pin1 = 1, Pin2 = 2, Pin3 = 3 };

    explicit sensors(QObject *parent = 0);
    sensors(sensors const &s);

    void insertSensor(int i ,QString serial);
    void replaceSensor(int i ,QString serial);
    QStringList getList();
    int length();
    QString sensorAt(int index);

signals:

public slots:

private:
    QStringList *sensorList;
};

#endif // SENSORS_H
