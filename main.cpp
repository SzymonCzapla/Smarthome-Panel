#include "mainwindow.h"
#include "networkhandler.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NetworkHandler* networkHandler = new NetworkHandler();
    MainWindow w(networkHandler);
    w.show();
    return a.exec();

}
