#ifndef UART_H
#define UART_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QString>
#include <QByteArray>
#include <QTimer>
#include "settingsdialog.h"

class UART : public QObject
{
    Q_OBJECT

signals:
    void readyRead(QByteArray data);
    void error(QString error);
    void isOpened();
    void isClosed();

public:
    UART(SettingsDialog::Settings s);

    bool isOpen();

public slots:
    void openSerialPort();
    void closeSerialPort();
    void write(const QByteArray &data);
    void newLine();


private slots:
    void readData();
    void changeSettings(SettingsDialog::Settings s);

    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serial;
    SettingsDialog::Settings settings;

};

#endif // UART_H
