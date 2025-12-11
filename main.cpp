#include "mainwindow.h"

#include <QApplication>
#include <QMetaType>
#include "flight.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<std::vector<Flight>>("std::vector<Flight>");

    MainWindow w;
    w.show();
    return a.exec();
}
