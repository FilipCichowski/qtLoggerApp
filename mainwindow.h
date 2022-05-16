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

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_editEntry_clicked();
    void on_addEntry_clicked();
    void on_deleteEntry_clicked();
    void on_openQRZDatabase_clicked();
    void tableViewClicked();

private:
    int activeRowIndex;
    Ui::MainWindow *ui;
    QString tableName;
    TableData activeRow;
    TableData insertData;
    void updateTable();
    void updateActiveRow();
    void showMessageBox(QString &message);
    void validateUserInput();
    void clearTextFields();
    void updateDateTime();
    void initWidgets();
};
#endif // MAINWINDOW_H
