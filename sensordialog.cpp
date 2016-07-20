#include "sensordialog.h"
#include "ui_sensordialog.h"

#include <QApplication>

#include <QDebug>

MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit(parent)
{

}

void MyLineEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    default: break;
    }
}

sensor::sensor(QString serial, int temp)
{
    this->serial = serial;
    this->temp = temp;
}

void sensor::setSerial(QString serial)
{
    this->serial = serial;
}

void sensor::setTemp(int temp)
{
    this->temp = temp;
}

QString sensor::getSerial()
{
    return serial;
}

int sensor::getTemp()
{
    return temp;
}

SensorDialog::SensorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorDialog)
{
    ui->setupUi(this);

    ui->readButton->setFocusPolicy(Qt::NoFocus);

    lineList = new QList<MyLineEdit*>;

    mon1Front = new QList<sensor*>;
    mon1Back = new QList<sensor*>;
    mon2Front = new QList<sensor*>;
    mon2Back = new QList<sensor*>;
    other = new QList<sensor*>;

    //savePath =  QApplication::applicationDirPath().left(1) + ":/sensors.ini";
    savePath = "C:/QT 5.6 workspace/build-Tmess-Desktop_Qt_5_6_0_MinGW_32bit-Debug/sensors.ini";
    //qDebug() << savePath;

    saveGroup1 = "Monitor_1";
    saveGroup2 = "Monitor_2";
    saveGroup3 = "Sonstige";
    front = "Front";
    back = "Back";

    connect(ui->applyButton, &QPushButton::clicked,
            this, &SensorDialog::apply);
    connect(ui->readButton, &QPushButton::clicked,
            this, &SensorDialog::readSerialClicked);
    connect(ui->saveButton, &QPushButton::clicked,
            this, &SensorDialog::saveSerials);
    connect(ui->openButton, &QPushButton::clicked,
            this, &SensorDialog::openSerials);

    setOrder();
    generateList();
    init();

    openSerials();
}

SensorDialog::~SensorDialog()
{
    delete ui;
}

QList<sensor*> SensorDialog::getAllSensors()
{
     QList<sensor*> allSensors;

     allSensors.append(*mon1Front);
     allSensors.append(*mon1Back);

     allSensors.append(*mon2Front);
     allSensors.append(*mon2Back);

     allSensors.append(*other);

     return allSensors;
}

void SensorDialog::putSerial(QString serial)
{
    sensor *s = new sensor(serial);

    QList<MyLineEdit*>::iterator i;
    for(i = lineList->begin(); i != lineList->end(); i++)
    {
        if( *i == focusWidget())
        {
            MyLineEdit *currentEdit = *i;
            currentEdit->setText(serial);
            //qDebug() << lineList->indexOf(*i);

            int index = lineList->indexOf(*i);
            int offsetL = 0;
            int offsetH = SensCount::Front;

            if(index >= offsetL && index < offsetH)         // 0 to 8
                mon1Front->replace(index, s);

            offsetL = offsetH;                              // 9
            offsetH = offsetL + SensCount::Back;            // 9 + 3

            if(index >= offsetL && index < offsetH)         // 9 to 11
                mon1Back->replace(index, s);

            offsetL = offsetH;                              // 12
            offsetH = offsetL + SensCount::Front;           // 12 + 9

            if(index >= offsetL && index < offsetH)         // 12 to 20
                mon2Front->replace(index, s);

            offsetL = offsetH;                              // 21
            offsetH = offsetL + SensCount::Back;            // 21 + 3

            if(index >= offsetL && index < offsetH)         // 21 to 23
                 mon2Back->replace(index, s);

            offsetL = offsetH;                              // 24
            offsetH = offsetL + SensCount::Other;           // 24 + 2

            if(index >= offsetL && index < offsetH)         // 24 to 25
                other->replace(index, s);
        }
    }
}

void SensorDialog::apply()
{
    hide();
    emit sensorsChanged();
}

void SensorDialog::readSerialClicked()
{
    emit requestSerial();
}

void SensorDialog::openSerials()
{
    QSettings open(savePath, QSettings::IniFormat);
    int offset = 0;
    sensor *s;
    QString serial;

    open.beginGroup(saveGroup1);
    open.beginGroup(front);
    for(int i = 0; i < SensCount::Front; i++)
    {
        serial = open.value(QString("%1").arg(i)).toString();
        s = new sensor(serial);
        mon1Front->replace(i, s);
        lineList->at(i + offset)->setText(serial);
    }
    open.endGroup();

    offset += SensCount::Front;

    open.beginGroup(back);
    for(int i = 0; i < SensCount::Back; i++)
    {
        serial = open.value(QString("%1").arg(i)).toString();
        s = new sensor(serial);
        mon1Back->replace(i, s);
        lineList->at(i + offset)->setText(serial);
    }
    open.endGroup();
    open.endGroup();

    offset += SensCount::Back;

    open.beginGroup(saveGroup2);
    open.beginGroup(front);
    for(int i = 0; i < SensCount::Front; i++)
    {
        serial = open.value(QString("%1").arg(i)).toString();
        s = new sensor(serial);
        mon2Front->replace(i, s);
        lineList->at(i + offset)->setText(serial);
    }
    open.endGroup();

    offset += SensCount::Front;

    open.beginGroup(back);
    for(int i = 0; i < SensCount::Back; i++)
    {
        serial = open.value(QString("%1").arg(i)).toString();
        s = new sensor(serial);
        mon2Back->replace(i, s);
        lineList->at(i + offset)->setText(serial);
    }
    open.endGroup();
    open.endGroup();

    offset += SensCount::Back;

    open.beginGroup(saveGroup3);
    for(int i = 0; i < SensCount::Other; i++)
    {
        serial = open.value(QString("%1").arg(i)).toString();
        s = new sensor(serial);
        other->replace(i, s);
        lineList->at(i + offset)->setText(serial);
    }
    open.endGroup();
}

void SensorDialog::saveSerials()
{
    QSettings save(savePath, QSettings::IniFormat);
    QList<sensor*>::Iterator i;
    sensor *s;
    int j = 0;

    save.beginGroup(saveGroup1);
    save.beginGroup(front);
    for(i = mon1Front->begin(); i != mon1Front->end(); i++)
    {
        s = *i;
        save.setValue(QString("%1").arg(j), s->getSerial());
        j++;
    }
    j = 0;
    save.endGroup();

    save.beginGroup(back);
    for(i = mon1Back->begin(); i != mon1Back->end(); i++)
    {
        s = *i;
        save.setValue(QString("%1").arg(j), s->getSerial());
        j++;
    }
    j = 0;
    save.endGroup();
    save.endGroup();

    save.beginGroup(saveGroup2);
    save.beginGroup(front);
    for(i = mon2Front->begin(); i != mon2Front->end(); i++)
    {
        s = *i;
        save.setValue(QString("%1").arg(j), s->getSerial());
        j++;
    }
    j = 0;
    save.endGroup();

    save.beginGroup(back);
    for(i = mon2Back->begin(); i != mon2Back->end(); i++)
    {
        s = *i;
        save.setValue(QString("%1").arg(j), s->getSerial());
        j++;
    }
    j = 0;
    save.endGroup();
    save.endGroup();

    save.beginGroup(saveGroup3);
    for(i = other->begin(); i != other->end(); i++)
    {
        s = *i;
        save.setValue(QString("%1").arg(j), s->getSerial());
        j++;
    }
    j = 0;
    save.endGroup();

    //qDebug() << save.allKeys();
}

void SensorDialog::init()
{
    for(int i = 0; i < SensCount::Front; i++)
    {
        sensor *s1 = new sensor("0");
        sensor *s2 = new sensor("0");
        mon1Front->append(s1);
        mon2Front->append(s2);
    }

    for(int i = 0; i < SensCount::Back; i++)
    {
        sensor *s1 = new sensor("0");
        sensor *s2 = new sensor("0");
        mon1Back->append(s1);
        mon2Back->append(s2);
    }

    for(int i = 0; i < SensCount::Other; i++)
    {
        sensor *s = new sensor("0");
        other->append(s);
    }
}

void SensorDialog::setOrder()
{
    setTabOrder(ui->lineEdit_11, ui->lineEdit_12);
    setTabOrder(ui->lineEdit_12, ui->lineEdit_13);
    setTabOrder(ui->lineEdit_13, ui->lineEdit_14);
    setTabOrder(ui->lineEdit_14, ui->lineEdit_15);
    setTabOrder(ui->lineEdit_15, ui->lineEdit_16);
    setTabOrder(ui->lineEdit_16, ui->lineEdit_17);
    setTabOrder(ui->lineEdit_17, ui->lineEdit_18);
    setTabOrder(ui->lineEdit_18, ui->lineEdit_19);

    setTabOrder(ui->lineEdit_19, ui->lineEdit_110);
    setTabOrder(ui->lineEdit_110, ui->lineEdit_111);
    setTabOrder(ui->lineEdit_111, ui->lineEdit_112);

    setTabOrder(ui->lineEdit_112, ui->lineEdit_21);

    setTabOrder(ui->lineEdit_21, ui->lineEdit_22);
    setTabOrder(ui->lineEdit_22, ui->lineEdit_23);
    setTabOrder(ui->lineEdit_23, ui->lineEdit_24);
    setTabOrder(ui->lineEdit_24, ui->lineEdit_25);
    setTabOrder(ui->lineEdit_25, ui->lineEdit_26);
    setTabOrder(ui->lineEdit_26, ui->lineEdit_27);
    setTabOrder(ui->lineEdit_27, ui->lineEdit_28);
    setTabOrder(ui->lineEdit_28, ui->lineEdit_29);

    setTabOrder(ui->lineEdit_29, ui->lineEdit_210);
    setTabOrder(ui->lineEdit_210, ui->lineEdit_211);
    setTabOrder(ui->lineEdit_211, ui->lineEdit_212);

    setTabOrder(ui->lineEdit_212, ui->lineEdit_31);
    setTabOrder(ui->lineEdit_31, ui->lineEdit_32);
}

void SensorDialog::generateList()
{
    lineList->append(ui->lineEdit_11);
    lineList->append(ui->lineEdit_12);
    lineList->append(ui->lineEdit_13);
    lineList->append(ui->lineEdit_14);
    lineList->append(ui->lineEdit_15);
    lineList->append(ui->lineEdit_16);
    lineList->append(ui->lineEdit_17);
    lineList->append(ui->lineEdit_18);
    lineList->append(ui->lineEdit_19);

    lineList->append(ui->lineEdit_110);
    lineList->append(ui->lineEdit_111);
    lineList->append(ui->lineEdit_112);

    lineList->append(ui->lineEdit_21);
    lineList->append(ui->lineEdit_22);
    lineList->append(ui->lineEdit_23);
    lineList->append(ui->lineEdit_24);
    lineList->append(ui->lineEdit_25);
    lineList->append(ui->lineEdit_26);
    lineList->append(ui->lineEdit_27);
    lineList->append(ui->lineEdit_28);
    lineList->append(ui->lineEdit_29);

    lineList->append(ui->lineEdit_210);
    lineList->append(ui->lineEdit_211);
    lineList->append(ui->lineEdit_212);

    lineList->append(ui->lineEdit_31);
    lineList->append(ui->lineEdit_32);
}
