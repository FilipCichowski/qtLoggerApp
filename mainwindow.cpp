#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDateTime>
#include "Data.h"
#include "./ui_mainwindow.h"
#include "Utility.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    extern const QStringList countryList, bands;
    extern const DatabaseData databaseData;
    startTimer(1000);
    ui->setupUi(this);
    initWidgets();
    Utility::selectTable(tableName);
    updateTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWidgets() {
    ui->country->addItems(countryList);
    ui->band->addItems(bands);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->timeEdit->setDisplayFormat("hh:mm:ss");
    updateDateTime();
}

void MainWindow::updateTable() {
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(databaseData.tableName);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0); // don't show the ID
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
            .id = model->index(activeRowIndex, 0).data().toString(),
            .call = model->index(activeRowIndex, 1).data().toString(),
            .name = model->index(activeRowIndex, 2).data().toString(),
            .country = model->index(activeRowIndex, 3).data().toString(),
            .utc = model->index(activeRowIndex, 4).data().toString(),
            .date = model->index(activeRowIndex, 5).data().toString(),
            .frequency = model->index(activeRowIndex, 6).data().toString(),
            .qslString = model->index(activeRowIndex, 7).data().toString()
    };
    qDebug() << model->index(activeRowIndex, 0).data().toString();
}

void MainWindow::updateDateTime() {
    auto DateTime = QDateTime::currentDateTimeUtc();
    ui->timeEdit->setTime(DateTime.time());
    ui->dateEdit->setDate(DateTime.date());
}

void MainWindow::on_addEntry_clicked()
{
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

    QString validateMessage = Utility::validateUserInput(insertData);

    if(validateMessage.length() == 0) {
        Utility::writeToDatabase(insertData, databaseData.tableName);
        clearTextFields();
        updateTable();
    } else {
        showMessageBox(validateMessage);
    }
}

void MainWindow::on_editEntry_clicked()
{
    Utility::updateRowByID(activeRow, databaseData.tableName);
}


void MainWindow::on_deleteEntry_clicked()
{
    Utility::deleteRowByUTCandDate(activeRow.utc, activeRow.date, databaseData.tableName);
    updateTable();
}

void MainWindow::tableViewClicked() {
   updateActiveRow();
}

void MainWindow::on_openQRZDatabase_clicked()
{
    QString url = "https://www.qrz.com/lookup/" + activeRow.call;
    qDebug() << url;
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}

void MainWindow::timerEvent(QTimerEvent *event) {
    updateDateTime();
}

