#include <QSqlQueryModel>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDateTime>
#include <QKeyEvent>
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
}

void MainWindow::updateDateTime() {
    auto DateTime = QDateTime::currentDateTimeUtc();
    ui->timeEdit->setTime(DateTime.time());
    ui->dateEdit->setDate(DateTime.date());
}

void MainWindow::on_addEntry_clicked() {
   addEntry();
}

void MainWindow::on_editEntry_clicked() {
    Utility::updateRowByID(activeRow, databaseData.tableName);
    updateTableView();
}

void MainWindow::on_deleteEntry_clicked() {
    Utility::deleteRowByUTCandDate(activeRow.utc, activeRow.date, databaseData.tableName);
    updateTableView();
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
                    case Qt::Key_E:
                        Utility::updateRowByID(activeRow, databaseData.tableName);
                        updateTableView();
                        break;
                    case Qt::Key_D:
                    Utility::deleteRowByUTCandDate(activeRow.utc, activeRow.date, databaseData.tableName);
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

    QString validateMessage = Utility::validateUserInput(insertData);

    if(validateMessage.length() == 0) {
        Utility::writeToDatabase(insertData, databaseData.tableName);
        clearTextFields();
        updateTableView();
    } else {
        showMessageBox(validateMessage);
    }
}



