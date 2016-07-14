#include "COM_handler.h"

#include <QDebug>

COM_handler::COM_handler(UART *uart)
{
    this->uart = uart;
    log = new QByteArray();
    readID = new QByteArray();
    sendID = new QByteArray();
}

void COM_handler::putData(const QByteArray &data)
{
    log->append(data);

    if( log->contains( QByteArray("id: ") ) )
    {
        idStart = true;
    }

    if( idStart && log->contains( QByteArray("rdyread") ) )
    {
        char *logdata = log->data();
        logdata = logdata + log->indexOf("id: ");
        int i = 0;
        readID->remove(0, readID->length());

        //qDebug() << "index : " << log->indexOf("id: ");

        while(*logdata)
        {
            //qDebug() << i << " : [" << *logdata << "]" ;

            if(i == 6 || i == 7 ){
                readID->append(*logdata);
            }


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

        qDebug() << "readID : " << *readID << " ; crc :" << lastCRC ;
    }

    if( log->contains( QByteArray("id?") ) )
    {
        sendID = readID; // debug

        idMeas = true;
        qDebug() << "sendID: " << *sendID;
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
    }

}

void COM_handler::connected()
{
    uart->write("h");
}

void COM_handler::messung()
{
    uart->write("m");
}

