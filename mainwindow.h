#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Structures.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_editEntry_clicked();
    void on_addEntry_clicked();
    void on_deleteEntry_clicked();
    void tableViewClicked();
    void on_openQRZDatabase_clicked();

private:
    int activeRowIndex;
    Ui::MainWindow *ui;
    QString tableName;
//    QString activeRowTime;
//    QString activeRowDate;
//    QString activeRowCall;
    TableData activeRow;
    TableData insertData;
    void updateTableUI();
    void updateActiveRow();
    void showMessageBox(QString &message);
    void validateUserInput();
    void clearTextFields();
};
#endif // MAINWINDOW_H
