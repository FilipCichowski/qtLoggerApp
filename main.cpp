#include "mainwindow.h"
#include "database.h"
#include "data.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(Database::isMySqlDatabaseConnected(databaseData)) {
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        qDebug() << "Can't open database";
        return -1;
    }
}
