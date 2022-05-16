#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QMessageBox>
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
    //qDebug() << "init" << tableName;
    ui->setupUi(this);
    ui->country->addItems(countryList);
    ui->band->addItems(bands);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    Utility::selectTable(tableName);
    //qDebug() << Utility::selectTable(tableName);
    this->updateTableUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTableUI() {
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable(databaseData.tableName);
    model->select();
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0); // don't show the ID
    ui->tableView->show();
//    QTableView *view = new QTableView;
//    view->setModel(model);
//    view->hideColumn(0); // don't show the ID
//    view->show();
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
            .qslString = (qslState ? "1" : "0")};

    QString validateMessage = Utility::validateUserInput(insertData);

    if(validateMessage.length() == 0) {
        Utility::writeToDatabase(insertData, databaseData.tableName);
        clearTextFields();
        updateTableUI();
    } else {
        showMessageBox(validateMessage);
    }
}

void MainWindow::on_editEntry_clicked()
{

}


void MainWindow::on_deleteEntry_clicked()
{
    Utility::deleteRowByUTCandDate(activeRow.utc, activeRow.date, databaseData.tableName);
    updateTableUI();
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

