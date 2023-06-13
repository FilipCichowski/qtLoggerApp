#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>

struct TableData {
    QString call, name, country, utc, date, frequency, qslString;
};

class DatabaseData {
    public:
        QString databaseName, tableName, hostName, userName, databasePassword;
        int port;
        DatabaseData(QString databaseName, QString tableName, QString hostName, QString userName, QString databasePassword, int port)
             : databaseName(databaseName),
               tableName(tableName),
               hostName(hostName),
               userName(userName),
               databasePassword(databasePassword),
               port(port){};
};

#endif // STRUCTURES_H
