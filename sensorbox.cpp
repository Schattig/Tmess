#include "sensorbox.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QSize>
#include <QString>

SensorBox::SensorBox(QString name, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *wrapper = new QHBoxLayout;
    QGroupBox *mainBox = new QGroupBox(name);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *topLayout = new QHBoxLayout;
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    QComboBox *pinSelect = new QComboBox;
    QCheckBox *checkActive = new QCheckBox;
    QGroupBox *frontBox = new QGroupBox("Front");
    QGroupBox *rearBox = new QGroupBox("Haube");
    QGridLayout *frontLayout = new QGridLayout;
    QVBoxLayout *rearLayout = new QVBoxLayout;

    QSize sizeTemp = QSize(30,20);
    mainBox->setFixedHeight(200);

    int row = 0;
    int column = 0;
    for(int i = 0; i < NumSensorF; i++)
    {
        if(i % 3 == 0)
        {
            row++;
            column = 0;
        }

        sensorsF[i] = new QLineEdit;
        sensorsF[i]->setFixedSize(sizeTemp);
        frontLayout->addWidget(sensorsF[i], row, column);

        column++;
    }
    for(int i = 0; i < NumSensorB; i++)
    {
        sensorsB[i] = new QLineEdit;
        sensorsB[i]->setFixedSize(sizeTemp);
        rearLayout->addWidget(sensorsB[i]);
    }

    rearBox->setLayout(rearLayout);
    frontBox->setLayout(frontLayout);

    bottomLayout->addWidget(frontBox);
    bottomLayout->addWidget(rearBox);

    topLayout->addWidget(pinSelect);
    topLayout->addWidget(checkActive);

    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomLayout);

    mainBox->setLayout(mainLayout);

    wrapper->addWidget(mainBox);

    setLayout(wrapper);

    setEnabled(false);

    connect(checkActive, &QCheckBox::stateChanged, this, &SensorBox::checkActiveChanged);
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
        clearTemp();
        emit checkChanged(false);
        enabled = false;
    }

}

void SensorBox::setEnabled(bool enable)
{
    for(int i = 0 ; i < NumSensorF; i++)
    {
        sensorsF[i]->setEnabled(enable);
    }

    for(int i = 0 ; i < NumSensorB; i++)
    {
        sensorsB[i]->setEnabled(enable);
    }
}

bool SensorBox::isEnabled()
{
    return enabled;
}

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

void SensorBox::clearTemp()
{
    for(int i = 0 ; i < NumSensorF; i++)
    {
        sensorsF[i]->setText("");
    }

    for(int i = 0 ; i < NumSensorB; i++)
    {
        sensorsB[i]->setText("");
    }
}
