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

SensorDialog::SensorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SensorDialog)
{
    ui->setupUi(this);

    ui->readButton->setFocusPolicy(Qt::NoFocus);

    lineList = new QList<MyLineEdit*>;

    //savePath =  QApplication::applicationDirPath().left(1) + ":/sensors.ini";
    savePath = "C:/QT 5.6 workspace/build-Tmess-Desktop_Qt_5_6_0_MinGW_32bit-Debug/sensors.ini";
    //qDebug() << savePath;

    //save = new QSettings(savePath, QSettings::IniFormat);
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

SensorDialog::monitor SensorDialog::getMonitor1() const
{
    return sensMon1;
}

SensorDialog::monitor SensorDialog::getMonitor2() const
{
    return sensMon2;
}

sensors SensorDialog::getOther() const
{
    return sensOther;
}

void SensorDialog::putSerial(QString serial)
{
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
                sensMon1.front.replaceSensor(index, serial);

            offsetL = offsetH;                              // 9
            offsetH = offsetL + SensCount::Back;            // 9 + 3

            if(index >= offsetL && index < offsetH)         // 9 to 11
                sensMon1.back.replaceSensor(index, serial);

            offsetL = offsetH;                              // 12
            offsetH = offsetL + SensCount::Front;           // 12 + 9

            if(index >= offsetL && index < offsetH)         // 12 to 20
                sensMon2.front.replaceSensor(index, serial);

            offsetL = offsetH;                              // 21
            offsetH = offsetL + SensCount::Back;            // 21 + 3

            if(index >= offsetL && index < offsetH)         // 21 to 23
                sensMon2.back.replaceSensor(index, serial);

            offsetL = offsetH;                              // 24
            offsetH = offsetL + SensCount::Other;           // 24 + 2

            if(index >= offsetL && index < offsetH)         // 24 to 25
                sensOther.replaceSensor(index, serial);

        }
    }
}

void SensorDialog::apply()
{
    hide();
}

void SensorDialog::readSerialClicked()
{
    emit requestSerial();
}

void SensorDialog::openSerials()
{
    QSettings open(savePath, QSettings::IniFormat);
    int offset = 0;

    open.beginGroup(saveGroup1);
    open.beginGroup(front);
    for(int i = 0; i < SensCount::Front; i++)
    {
        QString s = open.value(QString("%1").arg(i)).toString();
        sensMon1.front.replaceSensor(i, s);
        lineList->at(i + offset)->setText(s);
    }
    open.endGroup();

    offset += SensCount::Front;

    open.beginGroup(back);
    for(int i = 0; i < SensCount::Back; i++)
    {
        QString s = open.value(QString("%1").arg(i)).toString();
        sensMon1.back.replaceSensor(i, s);
        lineList->at(i + offset)->setText(s);
    }
    open.endGroup();
    open.endGroup();

    offset += SensCount::Back;

    open.beginGroup(saveGroup2);
    open.beginGroup(front);
    for(int i = 0; i < SensCount::Front; i++)
    {
        QString s = open.value(QString("%1").arg(i)).toString();
        sensMon2.front.replaceSensor(i, s);
        lineList->at(i + offset)->setText(s);
    }
    open.endGroup();

    offset += SensCount::Front;

    open.beginGroup(back);
    for(int i = 0; i < SensCount::Back; i++)
    {
        QString s = open.value(QString("%1").arg(i)).toString();
        sensMon2.back.replaceSensor(i, s);
        lineList->at(i + offset)->setText(s);
    }
    open.endGroup();
    open.endGroup();

    offset += SensCount::Back;

    open.beginGroup(saveGroup3);
    for(int i = 0; i < SensCount::Other; i++)
    {
        QString s = open.value(QString("%1").arg(i)).toString();
        sensOther.replaceSensor(i, s);
        lineList->at(i + offset)->setText(s);
    }
    open.endGroup();
}

void SensorDialog::saveSerials()
{
    QSettings save(savePath, QSettings::IniFormat);

    save.beginGroup(saveGroup1);
    save.beginGroup(front);
    for(int i = 0; i < sensMon1.front.length(); i++)
    {
        save.setValue(QString("%1").arg(i), sensMon1.front.sensorAt(i));
    }
    save.endGroup();

    save.beginGroup(back);
    for(int i = 0; i < sensMon1.back.length(); i++)
    {
        save.setValue(QString("%1").arg(i), sensMon1.back.sensorAt(i));
    }
    save.endGroup();
    save.endGroup();

    save.beginGroup(saveGroup2);
    save.beginGroup(front);
    for(int i = 0; i < sensMon2.front.length(); i++)
    {
        save.setValue(QString("%1").arg(i), sensMon2.front.sensorAt(i));
    }
    save.endGroup();

    save.beginGroup(back);
    for(int i = 0; i < sensMon2.back.length(); i++)
    {
        save.setValue(QString("%1").arg(i), sensMon2.back.sensorAt(i));
    }
    save.endGroup();
    save.endGroup();

    save.beginGroup(saveGroup3);
    for(int i = 0; i < sensOther.length(); i++)
    {
        save.setValue(QString("%1").arg(i), sensOther.sensorAt(i));
    }
    save.endGroup();

    //qDebug() << save.allKeys();
}

void SensorDialog::init()
{
    QString initVal = "0";

    for(int i = 0; i < SensCount::Front; i++)
    {
        sensMon1.front.insertSensor(i, initVal);
        sensMon2.front.insertSensor(i, initVal);
    }

    for(int i = 0; i < SensCount::Back; i++)
    {
        sensMon1.back.insertSensor(i, initVal);
        sensMon2.back.insertSensor(i, initVal);
    }

    for(int i = 0; i < SensCount::Other; i++)
    {
        sensOther.insertSensor(i, initVal);
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
