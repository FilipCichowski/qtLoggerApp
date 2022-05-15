#include "Utility.h"

bool Utility::isMySqlDatabaseConnected(DatabaseData databaseData) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(databaseData.hostName);
    db.setDatabaseName(databaseData.databaseName);
    db.setUserName(databaseData.userName);
    db.setPassword(databaseData.databasePassword);
//    qDebug() << databaseData.hostName;
//    qDebug() << databaseData.databaseName;
//    qDebug() << databaseData.userName;
//    qDebug() << databaseData.databasePassword;
    return db.open();
}

bool Utility::selectTable(QString const &tableName) {
    QSqlQuery query;
    query.prepare("USE " + tableName);
    return query.exec();
};

bool Utility::writeToDatabase(TableData &data, QString const &tableName) {
    QSqlQuery query;
    query.prepare("INSERT INTO " + tableName +  " (Callsign, Name, Country, UTC, Date, Frequency, QSL) VALUES (\"" + data.call + "\", \"" + data.name + "\", \"" + data.country + "\", \""  + data.utc + "\", \"" + data.date + "\", \"" + data.frequency + "\", \"" + data.qslString + "\")");
    return query.exec();
};

bool Utility::deleteRowByUTC(QString &utc, QString const &tableName) {
    QSqlQuery query;
    query.prepare("DELETE FROM " + tableName + " WHERE UTC=\"" + utc + "\"");
    qDebug() << "DELETE FROM " + tableName + " WHERE UTC=\"" + utc + "\"";
    return query.exec();
};



