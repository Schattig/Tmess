#include "uart.h"
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QByteArray>

#include <QDebug>

UART::UART(SettingsDialog::Settings s)
{
    serial = new QSerialPort(this);

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &UART::handleError);

    connect(serial, &QSerialPort::readyRead, this, &UART::readData);

    changeSettings(s);
}

void UART::openSerialPort()
{
    serial->setPortName(settings.name);
    serial->setBaudRate(settings.baudRate);
    serial->setDataBits(settings.dataBits);
    serial->setParity(settings.parity);
    serial->setStopBits(settings.stopBits);
    serial->setFlowControl(settings.flowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
      QByteArray text;
      text.append("connecting to ");
      text.append(settings.name);
      text.append("... \n");
      emit readyRead(text);
      emit isOpened();
    } else {
      emit error( QString().append(serial->errorString()) );
    }
}

void UART::closeSerialPort()
{
    if (serial->isOpen())
    {
        serial->close();
        emit isClosed();
    }
}

void UART::changeSettings(SettingsDialog::Settings s)
{
    closeSerialPort();

    settings.name = s.name;
    settings.baudRate = s.baudRate;
    settings.stringBaudRate = s.stringBaudRate;
    settings.dataBits = s.dataBits;
    settings.stringDataBits = s.stringDataBits;
    settings.parity = s.parity;
    settings.stringParity = s.stringParity;
    settings.stopBits = s.stopBits;
    settings.stringStopBits = s.stopBits;
    settings.flowControl = s.flowControl;
    settings.stringFlowControl = s.stringFlowControl;
    settings.localEchoEnabled = s.localEchoEnabled;
}

void UART::handleError(QSerialPort::SerialPortError err)
{
    if (err == QSerialPort::ResourceError) {
        emit error( serial->errorString() );
        closeSerialPort();
    }
}

void UART::readData()
{
       QByteArray data = serial->readAll();
       qDebug() << data;
       emit readyRead(data);
}

void UART::write(const QByteArray &data)
{
    serial->write(data);
}

void UART::newLine()
{
    QByteArray a = QByteArray("\r\n");
    serial->write(a);
    //qDebug() << "newline : " << a;
}

bool UART::isOpen()
{
    return serial->isOpen();
}
