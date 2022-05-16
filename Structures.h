#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>

struct TableData {
    QString  id, call, name, country, utc, date, frequency, qslString;
};

class DatabaseData {
    public:
        QString databaseName, tableName, hostName, userName, databasePassword;
        DatabaseData(QString databaseName, QString tableName, QString hostName, QString userName, QString databasePassword)
             : databaseName(databaseName),
               tableName(tableName),
               hostName(hostName),
               userName(userName),
               databasePassword(databasePassword){};
};

#endif // STRUCTURES_H
