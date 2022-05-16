#include "Utility.h"

bool Utility::isMySqlDatabaseConnected(const DatabaseData &databaseData) {
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

    //    qDebug() << "INSERT INTO " + tableName +  " (Callsign, Name, Country, UTC, Date, Frequency, QSL) VALUES (\"" + data.call + "\", \"" + data.name + "\", \"" + data.country + "\", \""  + data.utc + "\", \"" + data.date + "\", \"" + data.frequency + "\", \"" + data.qslString + "\")";

    return query.exec();
};

bool Utility::deleteRowByUTCandDate(QString &utc, QString &date, QString const &tableName) {
    QSqlQuery query;
    query.prepare("DELETE FROM " + tableName + " WHERE UTC=\"" + utc + "\" AND " + "Date=\"" + date + "\"");
    //qDebug() << "DELETE FROM " + tableName + " WHERE UTC=\"" + utc + "\" AND " + "Date=\"" + date + "\"";
    return query.exec();
};

bool Utility::updateRowByID(TableData &data, QString const &tableName) {
    QSqlQuery query;
    query.prepare("UPDATE " + tableName + " SET Callsign=\"" + data.call + "\", Name=\"" + data.name + "\", Country=\"" + data.country + "\", UTC=\"" + data.utc + "\", Date=\"" + data.date + "\", Frequency=\"" + data.frequency + "\", QSL=\"" + data.qslString + "\" WHERE ID=\"" + data.id + "\"");

    qDebug() << "UPDATE " + tableName + "SET Callsign=\"" + data.call + "\", Name=\"" + data.name + "\", Country=\"" + data.country + "\", UTC=\"" + data.utc + "\", Date=\"" + data.date + "\", Frequency=\"" + data.frequency + "\", QSL=\"" + data.qslString + "\" WHERE ID=\"" + data.id + "\"";

    return query.exec();
}

QString Utility::validateUserInput(TableData &insertData) {
    QString message;
    bool isNameEmpty, isCallEmpty, isFreqEmpty, isNameTooLong, isCallTooLong, isFreqTooLong;
    int nameLength, callLength, freqLength;
    static QRegularExpression re("\\D"); // match anything but digit
    nameLength = insertData.name.length();
    callLength = insertData.call.length();
    freqLength = insertData.frequency.length();
    isNameEmpty = nameLength == 0;
    isCallEmpty = callLength == 0;
    isFreqEmpty = freqLength == 0;
    isNameTooLong = nameLength > 15;
    isCallTooLong = callLength > 15;
    isFreqTooLong = freqLength > 15;

    if(isCallEmpty or isNameEmpty or isFreqEmpty) {
        message += "Please fill the blank fields!\n";
    }

    if(isNameTooLong or isCallTooLong or isFreqTooLong) {
        message += "User input's length shouldn't exceed 15 characters.\n";
    }

    if (re.match(insertData.frequency).hasMatch()) {
        message += "Frequency field should contain only numbers.\n";
    }

    return message;
};



