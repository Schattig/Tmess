#include "sensorbox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QSize>
#include <QLabel>
#include <QString>

SensorBox::SensorBox(QString name, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *wrapper = new QHBoxLayout;
    QGroupBox *mainBox = new QGroupBox(name);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    //pinSelect = new QComboBox;
    QLabel *labelActive = new QLabel("aktiv:");
    QCheckBox *checkActive = new QCheckBox;
    QGroupBox *frontBox = new QGroupBox("Front");
    QGroupBox *rearBox = new QGroupBox("Haube");
    QGridLayout *frontLayout = new QGridLayout;
    QVBoxLayout *rearLayout = new QVBoxLayout;

    QSize sizeTemp = QSize(30,20);
    mainBox->setFixedHeight(200);

    int row = 0;
    int column = 0;
    for(int i = 0; i < SensorDialog::SensCount::Front; i++)
    {
        if(i % 3 == 0)
        {
            row++;
            column = 0;
        }

        sensorsF[i] = new QLineEdit;
        sensorsF[i]->setFixedSize(sizeTemp);
        sensorsF[i]->setReadOnly(true);
        frontLayout->addWidget(sensorsF[i], row, column);

        column++;
    }
    for(int i = 0; i < SensorDialog::SensCount::Back; i++)
    {
        sensorsB[i] = new QLineEdit;
        sensorsB[i]->setFixedSize(sizeTemp);
        sensorsB[i]->setReadOnly(true);
        rearLayout->addWidget(sensorsB[i]);
    }

    rearBox->setLayout(rearLayout);
    frontBox->setLayout(frontLayout);

    bottomLayout->addWidget(frontBox);
    bottomLayout->addWidget(rearBox);

    //topLayout->addWidget(pinSelect);
    topLayout->addWidget(labelActive);
    topLayout->addWidget(checkActive);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    mainBox->setLayout(mainLayout);

    wrapper->addWidget(mainBox);

    setLayout(wrapper);

    setEnabled(false);

    connect(checkActive, &QCheckBox::stateChanged, this, &SensorBox::checkActiveChanged);
    //connect(pinSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(portComboboxChanged(int)));
}

void SensorBox::fillFront(int temp[SensorDialog::SensCount::Front])
{
    for(int i = 0; i < SensorDialog::SensCount::Front; i++)
    {
        sensorsF[i]->setText(generateTemp(temp[i]));
    }
}

void SensorBox::fillBack(int temp[SensorDialog::SensCount::Back])
{
    for(int i = 0; i < SensorDialog::SensCount::Back; i++)
    {
        sensorsB[i]->setText(generateTemp(temp[i]));
    }
}

QString SensorBox::generateTemp(int temp)
{
    double dtemp;
    dtemp = static_cast<double>(temp);
    dtemp = dtemp / 10;
    return QString("%1").arg(dtemp);
}


void SensorBox::checkActiveChanged(int state)
{
    if(state == Qt::Checked)
    {
        //setEnabled(true);
        emit checkChanged(true);
        enabled = true;
    }
    if(state == Qt::Unchecked)
    {
        //setEnabled(false);
        clear();
        emit checkChanged(false);
        enabled = false;
    }

}


void SensorBox::setEnabled(bool enable)
{
    for(int i = 0 ; i < SensorDialog::SensCount::Front; i++)
    {
        sensorsF[i]->setEnabled(enable);
    }

    for(int i = 0 ; i < SensorDialog::SensCount::Back; i++)
    {
        sensorsB[i]->setEnabled(enable);
    }
}

bool SensorBox::isEnabled()
{
    return enabled;
}

/*
void SensorBox::writeTemp(int temp, int sensor, Position position)
{
    if(isEnabled() == false)
        return;

    if(position == Position::Front)
    {
        if(sensor >= 0 && sensor < NumSensorF)
        {
            sensorsF[sensor]->setText(QString().append(temp));
        }
    }

    if(position == Position::Back)
    {
        if(sensor >= 0 && sensor < NumSensorB)
        {
            sensorsB[sensor]->setText(QString().append(temp));
        }
    }
}
*/

void SensorBox::clear()
{
    for(int i = 0 ; i < SensorDialog::SensCount::Front; i++)
    {
        sensorsF[i]->setText("");
    }

    for(int i = 0 ; i < SensorDialog::SensCount::Back; i++)
    {
        sensorsB[i]->setText("");
    }
}

/*
void SensorBox::addPort(QString text, sensors::Port port)
{
    pinSelect->addItem(text, port);
}

void SensorBox::addPort(QList<sensorEntry> sensors)
{

    for(int i = 0; i < sensors.size(); i++)
    {
        addPort(sensors[i].name, sensors[i].port);
    }
}
*/

/*
int SensorBox::portComboboxChanged(int index)
{

    if(pinSelect->currentData() == sensors::Pin1)
        currentPort = sensors::Pin1;
    if(pinSelect->currentData() == sensors::Pin2)
        currentPort = sensors::Pin2;
    if(pinSelect->currentData() == sensors::Pin3)
        currentPort = sensors::Pin3;

    return index;


    return index;
}
*/

/*
sensors::Port SensorBox::getPort()
{
    return currentPort;
}
*/
