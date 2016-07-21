#ifndef SENSORDIALOG_H
#define SENSORDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QLineEdit>
#include <QList>
#include <QSettings>

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

class sensor
{
public:
    explicit sensor(QString serial, int temp = 0);

    void setSerial(QString serial);
    void setTemp(int temp);

    QString getSerial();
    int getTemp();

private:
    QString serial;
    int temp;
};

class SensorDialog : public QDialog
{
    Q_OBJECT

public:

    enum SensCount{
        Front = 9,
        Back = 3,
        Other = 2,
        All = 2*(Front + Back) + Other
    };

    explicit SensorDialog(QWidget *parent = 0);
    ~SensorDialog();

     void getAllSensors(QList<sensor*> *mon1F, QList<sensor*> *mon1B,
                        QList<sensor*> *mon2F, QList<sensor*> *mon2B,
                        QList<sensor*> *other);

     void initAllSensors(QList<sensor*> *mon1F, QList<sensor*> *mon1B,
                         QList<sensor*> *mon2F, QList<sensor*> *mon2B,
                         QList<sensor*> *other);

signals:
    void requestSerial();
    void sensorsChanged();

public slots:
    void putSerial(QString serial);

private slots:
    void apply();
    void readSerialClicked();
    void saveSerials();
    void openSerials();

private:
    void setOrder();
    void generateList();
    void init();
    Ui::SensorDialog *ui;
    QList<MyLineEdit*> *lineList;

    QList<sensor*> *mon1Front;
    QList<sensor*> *mon1Back;
    QList<sensor*> *mon2Front;
    QList<sensor*> *mon2Back;
    QList<sensor*> *other;

    QString savePath;
    QString saveGroup1;
    QString saveGroup2;
    QString saveGroup3;
    QString front;
    QString back;
};

#endif // SENSORDIALOG_H
