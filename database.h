#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <QRegularExpression>
#include <structures.h>


class Database {
    public:
        static bool isMySqlDatabaseConnected(const DatabaseData &databaseData);
        static bool selectTable(QString const &tableName);
        static bool writeToDatabase(TableData &data, QString const &tableName);
        static bool deleteRowByUtcAndDate(QString &utc, QString &date, QString const &tableName);
};
#endif
