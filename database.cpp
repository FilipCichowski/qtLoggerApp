#include "database.h"

bool Database::isMySqlDatabaseConnected(const DatabaseData &databaseData) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(databaseData.hostName);
    db.setPort(databaseData.port);
    db.setDatabaseName(databaseData.databaseName);
    db.setUserName(databaseData.userName);
    db.setPassword(databaseData.databasePassword);
    return db.open();
}

bool Database::selectTable(QString const &tableName) {
    QSqlQuery query;
    query.prepare("USE " + tableName);
    return query.exec();
};

bool Database::writeToDatabase(TableData &data, QString const &tableName) {
    QSqlQuery query;
    query.prepare("INSERT INTO " + tableName +  " (Callsign, Name, Country, UTC, Date, Frequency, QSL) VALUES (\"" + data.call + "\", \"" + data.name + "\", \"" + data.country + "\", \""  + data.utc + "\", \"" + data.date + "\", \"" + data.frequency + "\", \"" + data.qslString + "\")");
    return query.exec();
};

bool Database::deleteRowByUTCandDate(QString &utc, QString &date, QString const &tableName) {
    QSqlQuery query;
    query.prepare("DELETE FROM " + tableName + " WHERE UTC=\"" + utc + "\" AND " + "Date=\"" + date + "\"");
    return query.exec();
};



