#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QStringList>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlTableModel>
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
    void on_create_clicked();

    void on_save_clicked();

    void on_input_clicked();

    void on_show_clicked();

    void on_delete_2_clicked();

private:
    Ui::MainWindow *ui;
    void clearInputs();
    void initTableView();
};
#endif // MAINWINDOW_H
