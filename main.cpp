#include "mainwindow.h"
#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    /* ModBus protocol debug */
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
