#include "COM_handler.h"

#include <QDebug>

COM_handler::COM_handler(UART *uart, SensorDialog *sDialog)
{
    this->uart = uart;
    this->sDialog = sDialog;
    log = new QByteArray();
    readID = new QByteArray();
    sendID = new QByteArray();
    //allSensors = new QList<sensor*>;

    mon1F = new QList<sensor*>;
    mon1B = new QList<sensor*>;
    mon2F = new QList<sensor*>;
    mon2B = new QList<sensor*>;
    other = new QList<sensor*>;

    sDialog->initAllSensors(mon1F, mon1B, mon2F, mon2B, other);

    updateAllSensors();

    connect(uart, &UART::readyRead, this, &COM_handler::putData);
    connect(sDialog, SIGNAL(sensorsChanged()), this, SLOT(updateAllSensors()));
}

void COM_handler::putData(const QByteArray &data)
{
    log->append(data);

    if( log->contains( QByteArray("...connected") ))
    {
        //qDebug() << "connected";
        sendAllSensors();
        log->remove(0, log->length());
    }

    if( log->contains( QByteArray("case get all") ) && sendAll  )
    {
        QList<sensor*>::Iterator i;
        sensor *s;

        if( log->contains( QByteArray("mon1") ) && mon1)
        {
            for(i = mon1F->begin(); i != mon1F->end(); i++)
            {
                s = *i;
                sendSensor(s);
            }
            for(i = mon1B->begin(); i != mon1B->end(); i++)
            {
                s = *i;
                sendSensor(s);
            }
            mon1 = false;
        }

        if( log->contains( QByteArray("mon2") ) && mon2)
        {
            for(i = mon2F->begin(); i != mon2F->end(); i++)
            {
                s = *i;
                sendSensor(s);
            }
            for(i = mon2B->begin(); i != mon2B->end(); i++)
            {
                s = *i;
                sendSensor(s);
            }
            mon2 = false;
        }

        if( log->contains( QByteArray("other") ) && others)
        {
            for(i = other->begin(); i != other->end(); i++)
            {
                s = *i;
                sendSensor(s);
            }

            others = false;
        }

        if(!mon1 && !mon2) //&& !others)
        {
            sendAll = false;
            mon1 = true;
            mon2 = true;
            others = true;
            log->remove(0, log->length());
        }

        /*
        sendAll = false;

        QList<sensor*>::Iterator i;
        sensor *s;
        for(i = allSensors->begin(); i != allSensors->end(); i++)
        {
            QByteArray a;
            s = *i;
            QString serial = s->getSerial();
            if(serial == "")
                a = QByteArray("0000000000000000");
            else
                a = QByteArray().append(serial);

            qDebug() << a;
            uart->write(a);
            uart->newLine();
        }
        */
    }

    if( log->contains( QByteArray("id: ") ) && reqSerial  )
    {
        reqSerial = false;
        idStart = true;
    }

    if( log->contains( QByteArray("kein Sensor erkannt") ) && reqSerial  )
    {
        reqSerial = false;
        emit serialReady(QString("nicht erkannt"));
        log->remove(0, log->length());
    }

    if(log->contains( QByteArray("rdyread") ) &&  idStart )
    {
        char *logdata = log->data();
        logdata = logdata + log->indexOf("id: ");
        int i = 0;
        readID->remove(0, readID->length());

        //qDebug() << "index : " << log->indexOf("id: ");

        while(*logdata)
        {
            //qDebug() << i << " : [" << *logdata << "]" ;

            if(i == 6 || i == 7 )
                readID->append(*logdata);

            if(i >= 10 && i < 22)
                readID->append(*logdata);

            if(i == 24 || i == 25)
                readID->append(*logdata);

            if(i == 32)
                lastCRC = strcmp(logdata, "O");

            ++logdata;
            i++;
        }
        log->remove(0, log->length());
        idStart = false;

        //qDebug() << "readID : " << *readID << " ; crc :" << lastCRC ;

        if(lastCRC)
            emit serialReady(QString::fromStdString(readID->toStdString()));
        else
            emit serialReady(QString("crc fail"));
    }

    if( log->contains( QByteArray("id?") ))
    {
        sendID = readID; // debug

        idMeas = true;
        //qDebug() << "sendID: " << *sendID;
        uart->write(*sendID);
        log->remove(0, log->length());
    }

    if( idMeas && log->contains( QByteArray("rdymeas")) ){
        char *logdata = log->data();
        logdata = logdata + log->indexOf("temp: ");
        int i = 0;

        lastTemp.remove(0, lastTemp.length());

        while(*logdata)
        {
            //qDebug() << i << " : [" << *logdata << "]" ;

            if(i >=6 && i <=8 )
                lastTemp.append(*logdata);

            i++;
            ++logdata;
        }

        log->remove(0, log->length());
        idMeas = false;

        qDebug() << "Temp : " << lastTemp ;

        emit measReady(lastTemp);
    }
}

void COM_handler::connected()
{
    uart->write("h");
}

void COM_handler::messung()
{   
    //uart->write("m"); //single
    uart->write("l");   //all
}

void COM_handler::requestSerial()
{
    uart->write("s");
    reqSerial = true;
}

void COM_handler::sendAllSensors()
{
    sendAll = true;
    uart->write("a");
}

void COM_handler::sendSensor(sensor *s)
{
    QByteArray a;
    QString serial = s->getSerial();
    if(serial == "")
        a = QByteArray("0000000000000000");
    else
        a = QByteArray().append(serial);

    qDebug() << a;
    uart->write(a);
    uart->newLine();
}

void COM_handler::updateAllSensors()
{
    sDialog->getAllSensors(mon1F, mon1B, mon2F, mon2B, other);
    sendAllSensors();

    /* debug */
    /*
    qDebug() << "All Sensor Debug :";
    int j = 0;
    QList<sensor*>::Iterator i;
    for(i = allSensors->begin(); i != allSensors->end(); i++)
    {
        sensor *s = *i;
        qDebug() << j << s->getSerial();
        j++;
    }
    */

}

