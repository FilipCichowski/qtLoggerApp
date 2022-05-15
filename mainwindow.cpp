#include <QSqlQueryModel>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Data.h"
#include "Utility.h"
#include "Structures.h"

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
    ui->tableView->show();
    qDebug() << "refreshed";

//    QSqlQueryModel *model = new QSqlQueryModel();
//    QSqlQuery query;
//    query.prepare("select * from " + tableName);
//    qDebug() << "select * from " + tableName;
//    qDebug() << databaseData.tableName;
//    query.exec();
//    model->setQuery(query);
//    ui->tableView->setModel(model);
//    ui->tableView->show();
//    auto tableModel = new QSqlTableModel();
//    tableModel->setTable(tableName);
//    tableModel->select();
//    ui->tableView->setModel(tableModel);


}

void MainWindow::on_addEntry_clicked()
{
    qDebug() << "clicked" << tableName;
    bool qslState;
    TableData insertData;
    qslState = ui->qsl->isChecked();
    insertData = (TableData) {
            .call = ui->call->text(),
            .name = ui->call->text(),
            .country = ui->country->currentText(),
            .utc = ui->timeEdit->time().toString(),
            .date = ui->dateEdit->date().toString("yyyy-MM-dd"),
            .frequency = ui->frequency->text(),
            .qslString = (qslState ? "1" : "0")};

    Utility::writeToDatabase(insertData, databaseData.tableName);

    //clear every text field
    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>()) {
        widget->clear();
    }
    this->updateTableUI();
}

void MainWindow::on_editEntry_clicked()
{

}


void MainWindow::on_deleteEntry_clicked()
{
    Utility::deleteRowByUTC(activeRowTime, databaseData.tableName);
    this->updateTableUI();
}

void MainWindow::tableViewClicked() {
    activeRowIndex = ui->tableView->selectionModel()->currentIndex().row();
    activeRowTime = ui->tableView->model()->index(activeRowIndex, 3).data().toString();
}
