#include "sensordialog.h"
#include "ui_sensordialog.h"

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

    lineList = new QList<MyLineEdit*>;

    connect(ui->applyButton, &QPushButton::clicked,
            this, &SensorDialog::apply);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &SensorDialog::cancel);

    setOrder();
    generateList();
}

SensorDialog::~SensorDialog()
{
    delete ui;
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
        }
    }
}

void SensorDialog::apply()
{
    hide();
}

void SensorDialog::cancel()
{
    hide();
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
