#ifndef SENSORBOX_H
#define SENSORBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QList>

#include "sensordialog.h"

class SensorBox : public QWidget
{
    Q_OBJECT
public:
    //enum Position { Front, Back };

    /*
    struct sensorEntry{
        QString name;
        sensors::Port port;
    };
    */

    explicit SensorBox(QString name, QWidget *parent = 0);

    void setEnabled(bool enable);
    void fillFront(int temp[SensorDialog::SensCount::Front]);
    void fillBack(int temp[SensorDialog::SensCount::Back]);

    //void addPort(QString text, sensors::Port port);
    //void addPort(QList<sensorEntry> sensors);

    bool isEnabled();

    //sensors::Port getPort();

signals:
    void checkChanged(bool state);
    //void portChanged(sensors::Port);

private slots:
    void checkActiveChanged(int state);
    int portComboboxChanged(int index);

private:
    QLineEdit *sensorsF[SensorDialog::SensCount::Front];
    QLineEdit *sensorsB[SensorDialog::SensCount::Back];
    QComboBox *pinSelect;
    bool enabled = false;

    QString generateTemp(int temp);

    //sensors::Port currentPort;

    void clear();
};

#endif // SENSORBOX_H
