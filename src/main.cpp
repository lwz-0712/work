#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE","cc");
    db.setDatabaseName("baojia.db");
    if(!db.open())
    {
        qDebug() <<"报价打开失败";
    }
    else {
          qDebug() <<"报价打开cg";
    }
    w.show();
    return a.exec();
}
