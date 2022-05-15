#ifndef UTILITY_H
#define UTILITY_H

#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <Structures.h>

class Utility {
    public:
        static bool isMySqlDatabaseConnected(DatabaseData databaseData);
        static bool selectTable(QString const &tableName);
        static bool writeToDatabase(TableData &data, QString const &tableName);
        static bool deleteRowByUTC(QString &utc, QString const &tableName);
};
#endif // UTILITY_H
