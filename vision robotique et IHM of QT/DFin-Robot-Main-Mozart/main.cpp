#include <QApplication>
#include "mainwindow.h"
//#include <QtPlugin>

//Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
//Q_IMPORT_PLUGIN(AccessibleFactory)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
