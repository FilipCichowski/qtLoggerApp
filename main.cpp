#include "mainwindow.h"
#include "Utility.h"
#include "Data.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(Utility::isMySqlDatabaseConnected(databaseData)) {
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        qDebug() << "Can't open database";
        return -1;
    }
}
