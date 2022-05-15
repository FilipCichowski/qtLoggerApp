#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    QString tableName;
    void updateTableUI();
    int activeRowIndex;
    QString activeRowTime;
};
#endif // MAINWINDOW_H
