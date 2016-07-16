#include "modbus.h"
#include <QVariant>

#include <QDebug>

ModBus::ModBus(SettingsDialog::ModBusSettings s, QObject *parent)
    : QObject(parent)
    //, lastRequest(Q_NULLPTR)
    , modbusDevice(Q_NULLPTR)
{
    settings = s;

    modbusDevice = new QModbusRtuSerialMaster(this);

}

void ModBus::openModBusPort()
{
    if (!modbusDevice)
    {
        qDebug() << "open ModBus failed , !modBusDevice";
        return;
    }

    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
            QVariant(settings.name));

        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter,
           QVariant(settings.parity));

        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
           QVariant(settings.baud));

        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
            QVariant(settings.dataBits));

        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
           QVariant(settings.stopBits));

        modbusDevice->setTimeout(settings.responseTime);
        modbusDevice->setNumberOfRetries(settings.numberOfRetries);

        if (!modbusDevice->connectDevice()) {
            qDebug() << "ModBus connect failed";
        } else {
            emit isOpened();
        }
    } else {
        qDebug() << "ModBus allready connected";
    }

}

void ModBus::closeModBusPort()
{
    if (!modbusDevice)
    {
        qDebug() << "close ModBus failed , !modBusDevice";
        return;
    }

    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
        modbusDevice->disconnectDevice();
        emit isClosed();
    }
}

void ModBus::readData()
{
  int startAddress = 0;
  int numberOfEntries = 10;
  int serverAddress = 1;

  if(startAddress < 0 || startAddress >= 10)
  {
      qDebug() << "startAddress out of range";
      return;
  }

  QModbusDataUnit::RegisterType type = QModbusDataUnit::HoldingRegisters;

  Q_ASSERT(startAddress >= 0 && startAddress < 10);
  numberOfEntries = qMin(numberOfEntries, 10 - startAddress);

  QModbusDataUnit readRequest = QModbusDataUnit(type, startAddress, static_cast<quint16>(numberOfEntries));

  if(auto *reply = modbusDevice->sendReadRequest(readRequest, serverAddress))
  {
      if(!reply->isFinished())
          connect(reply, &QModbusReply::finished, this, &ModBus::readFinished);
      else
          delete reply;
  } else {
      qDebug() << "read error: " << modbusDevice->errorString();
  }
}

void ModBus::readFinished()
{
    QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;

     if (reply->error() == QModbusDevice::NoError) {
         const QModbusDataUnit data = reply->result();
         emit readyRead(data);

         qDebug() << "recieved ModBus data count: " << data.valueCount() ;
         qDebug() << "ModBus data : ";
         for (uint i = 0; i < data.valueCount(); i++) {
             qDebug() << data.value(static_cast<int>(i));
         }

     } else if (reply->error() == QModbusDevice::ProtocolError) {
         qDebug() << " Read response error : " << reply->errorString() << " (ModBus exception: "
                  << reply->rawResult().exceptionCode() << ")";
     } else {
          qDebug() << " Read response error : " << reply->errorString() << " (code: "
                   << reply->error() << ")";
     }

     reply->deleteLater();
}

void ModBus::changeSettings(SettingsDialog::ModBusSettings s)
{
    settings = s;
}



