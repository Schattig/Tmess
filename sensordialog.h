#ifndef SENSORDIALOG_H
#define SENSORDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
#include <QSettings>
#include "sensors.h"

namespace Ui {
class SensorDialog;
}

class MyLineEdit : public QLineEdit
{
public:
    explicit MyLineEdit(QWidget *parent = 0);
protected:
    virtual void keyPressEvent(QKeyEvent *e);
};

class SensorDialog : public QDialog
{
    Q_OBJECT

public:

    struct monitor{
        sensors front;
        sensors back;
    };

    explicit SensorDialog(QWidget *parent = 0);
    ~SensorDialog();

    monitor getMonitor1() const;
    monitor getMonitor2() const;
    sensors getOther() const;

signals:
    void requestSerial();

public slots:
    void putSerial(QString serial);

private slots:
    void apply();
    void readSerialClicked();
    void saveSerials();
    void openSerials();

private:

    enum SensCount{
        Front = 9,
        Back = 3,
        Other = 2
    };

    void setOrder();
    void generateList();
    void init();
    Ui::SensorDialog *ui;
    QList<MyLineEdit*> *lineList;

    monitor sensMon1;
    monitor sensMon2;
    sensors sensOther;

    //QSettings *save;
    QString savePath;
    QString saveGroup1;
    QString saveGroup2;
    QString saveGroup3;
    QString front;
    QString back;
};

#endif // SENSORDIALOG_H
