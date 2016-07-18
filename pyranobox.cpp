#include "pyranobox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>

#include <QDebug>

PyranoBox::PyranoBox(QString name, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *wrapper = new QHBoxLayout;
    QGroupBox *mainBox = new QGroupBox(name);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *wattLayout = new QHBoxLayout;
    QHBoxLayout *tempLayout = new QHBoxLayout;

    QLabel *watt = new QLabel("w/m² :");
    QLabel *temp = new QLabel("Temperatur °C:");
    showWatt = new QLineEdit;
    showTemp = new QLineEdit;

    watt->setFixedWidth(75);
    temp->setFixedWidth(75);

    showWatt->setFixedWidth(40);
    showTemp->setFixedWidth(40);

    wattLayout->addWidget(watt);
    wattLayout->addWidget(showWatt);
    wattLayout->addStretch();

    tempLayout->addWidget(temp);
    tempLayout->addWidget(showTemp);
    tempLayout->addStretch();

    mainLayout->addLayout(wattLayout);
    mainLayout->addLayout(tempLayout);

    mainBox->setLayout(mainLayout);

    wrapper->addWidget(mainBox);

    setLayout(wrapper);

    setEnabled(false);
}

void PyranoBox::writeWatt(int watt)
{
    //qDebug() << "watt P: " << watt;
    showWatt->setText(QString("%1").arg(watt));
}

void PyranoBox::writeTemp(int temp)
{
    //qDebug() << "temp P: " << temp;
    double dtemp = static_cast<double>(temp)/10;
    showTemp->setText(QString("%1").arg(dtemp));
}

void PyranoBox::clear()
{
    showWatt->setText("");
    showTemp->setText("");
}

void PyranoBox::setEnabled(bool enable)
{
    showWatt->setEnabled(enable);
    showTemp->setEnabled(enable);
}
