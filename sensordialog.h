#ifndef SENSORDIALOG_H
#define SENSORDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
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

signals:
    void requestSerial();

public slots:
    void putSerial(QString serial);

private slots:
    void apply();
    void cancel();
    void readSerialClicked();
    void saveSerials();
    void openSerials();

private:
    void setOrder();
    void generateList();
    Ui::SensorDialog *ui;
    QList<MyLineEdit*> *lineList;

    monitor *sensMon1;
    monitor *sensMon2;
    sensors *sensOther;
};

#endif // SENSORDIALOG_H
