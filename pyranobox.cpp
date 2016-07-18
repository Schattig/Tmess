#include "pyranobox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>


PyranoBox::PyranoBox(QString name, QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *wrapper = new QHBoxLayout;
    QGroupBox *mainBox = new QGroupBox(name);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *wattLayout = new QHBoxLayout;
    QHBoxLayout *tempLayout = new QHBoxLayout;

    QLabel *watt = new QLabel("w/m² :");
    QLabel *temp = new QLabel("Sensor Temperatur :");
    showWatt = new QLineEdit;
    showTemp = new QLineEdit;

    watt->setFixedWidth(100);
    temp->setFixedWidth(100);

    showWatt->setFixedWidth(75);
    showTemp->setFixedWidth(75);

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
    showWatt->setText(QString().append(watt));
}

void PyranoBox::writeTemp(int temp)
{
    showTemp->setText(QString().append(temp));
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
