#include "mainwindow.h"
#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    /* ModBus protocol debug */
    //QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));

    QCoreApplication::setOrganizationName("GEPRO GmbH");
    QCoreApplication::setApplicationName("Tmess");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
