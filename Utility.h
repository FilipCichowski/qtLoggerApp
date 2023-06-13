#ifndef UTILITY_H
#define UTILITY_H

#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <QRegularExpression>
#include <Structures.h>


class Utility {
    public:
        static bool isMySqlDatabaseConnected(const DatabaseData &databaseData);
        static bool selectTable(QString const &tableName);
        static bool writeToDatabase(TableData &data, QString const &tableName);
        static bool deleteRowByUTCandDate(QString &utc, QString &date, QString const &tableName);
        static QString validateUserInput(TableData &insertData);
};
#endif // UTILITY_H
