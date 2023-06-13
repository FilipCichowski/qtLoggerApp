#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDateTime>
#include <QKeyEvent>
#include <QFileDialog>
#include "./ui_mainwindow.h"
#include "database.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    startTimer(1000);
    ui->setupUi(this);
    initWidgets();
    Database::selectTable(tableName);
    updateTableView();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initWidgets() {
    ui->country->addItems(countryList);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->timeEdit->setDisplayFormat("hh:mm:ss");
    updateDateTime();
}

void MainWindow::updateTableView() {
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(databaseData.tableName);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();
}

void MainWindow::showMessageBox(QString &message) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.exec();
}

void MainWindow::clearTextFields() {
    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>()) {
        widget->clear();
    }
}

void MainWindow::updateActiveRow() {
    activeRowIndex = ui->tableView->selectionModel()->currentIndex().row();
    auto model = ui->tableView->model();
    activeRow = (TableData) {
            .call = model->index(activeRowIndex, 0).data().toString(),
            .name = model->index(activeRowIndex, 1).data().toString(),
            .country = model->index(activeRowIndex, 2).data().toString(),
            .utc = model->index(activeRowIndex, 3).data().toString(),
            .date = model->index(activeRowIndex, 4).data().toString(),
            .frequency = model->index(activeRowIndex, 5).data().toString(),
            .qslString = model->index(activeRowIndex, 6).data().toString()
    };
}

void MainWindow::updateDateTime() {
    auto DateTime = QDateTime::currentDateTimeUtc();
    ui->timeEdit->setTime(DateTime.time());
    ui->dateEdit->setDate(DateTime.date());
}

void MainWindow::on_addEntry_clicked() {
   addEntry();
}

void MainWindow::on_deleteEntry_clicked() {
    Database::deleteRowByUtcAndDate(activeRow.utc, activeRow.date, databaseData.tableName);
    updateTableView();
}

void MainWindow::on_exportEntry_clicked() {
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(databaseData.tableName);
    model->select();

    QString CSVString = "Call sign, Name, Country, UTC, Date, Frequency, QSL\n";

    for (int i = 0; i < model->rowCount(); i++) {
        for (int j = 0; j < model->columnCount(); j++) {
            CSVString += model->data(model->index(i,j)).toString();
            if (j != model->columnCount() - 1) CSVString += ",";
        }
        CSVString += "\n";
    }

    QString message;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "C://", tr("CSV (*.csv)"));
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << CSVString;
        file.close();

        message = "CSV file succesfully created.";
        showMessageBox(message);
    }
    else {
        message = "Couldn't create file.";
        showMessageBox(message);
    }
}

void MainWindow::tableViewClicked() {
   updateActiveRow();
}

void MainWindow::on_openQRZDatabase_clicked() {
    openQrz();
}

void MainWindow::timerEvent(QTimerEvent *event) {
    updateDateTime();
}

void MainWindow::keyPressEvent(QKeyEvent* f_event) {
    if(f_event->type() == QEvent::KeyPress)
        {
            if( f_event->modifiers().testFlag(Qt::ControlModifier) )
            {
                switch(f_event->key())
                {
                    case Qt::Key_S:
                        addEntry();
                        break;
                    case Qt::Key_D:
                        Database::deleteRowByUtcAndDate(activeRow.utc, activeRow.date, databaseData.tableName);
                        updateTableView();
                        break;
                    case Qt::Key_F:
                        openQrz();
                        break;
                }
            }
        }
}

void MainWindow::openQrz() {
    QString url = "https://www.qrz.com/lookup/" + activeRow.call;
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

void MainWindow::addEntry() {
    bool qslState;
    qslState = ui->qsl->isChecked();
    insertData = (TableData) {
            .call = ui->call->text(),
            .name = ui->name->text(),
            .country = ui->country->currentText(),
            .utc = ui->timeEdit->time().toString(),
            .date = ui->dateEdit->date().toString("yyyy-MM-dd"),
            .frequency = ui->frequency->text(),
            .qslString = (qslState ? "YES" : "NO")};

    QString validateMessage = validateUserInput(insertData);

    if(validateMessage.length() == 0) {
        Database::writeToDatabase(insertData, databaseData.tableName);
        clearTextFields();
        updateTableView();
    } else {
        showMessageBox(validateMessage);
    }
}

QString MainWindow::validateUserInput(TableData &insertData) {
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

