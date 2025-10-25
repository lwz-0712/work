#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_create_clicked()
{
    QSqlDatabase db=QSqlDatabase::database("cc");
    QSqlQuery query(db);
    bool ok = query.exec("CREATE TABLE IF NOT EXISTS quote_table ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "product_name VARCHAR(100) NOT NULL, "
                           "min_quantity DECIMAL(10,2) NOT NULL, "
                           "max_quantity DECIMAL(10,2), "
                           "price DECIMAL(10,2) NOT NULL)");

       if (ok) {
           qDebug() << "报价表创建成功！";
       } else {
           qDebug() << "报价表创建失败：" << query.lastError().text();
       }
   }

void MainWindow::on_save_clicked()
{
    QSqlDatabase db=QSqlDatabase::database("cc");
    QSqlQuery query(db);
    QString productName = ui->name2->text();        // 产品名称
       QString minQuantity = ui->min->text();        // 最小数量
       QString maxQuantity = ui->max->text();        // 最大数量
       QString price = ui->price->text();
       if (productName.isEmpty() || minQuantity.isEmpty() || price.isEmpty()) {
               QMessageBox::warning(this, "输入错误", "请填写产品名称、最小数量和价格！");
               return;
           }


       QString insertSQL;
          if (maxQuantity.isEmpty()) {
              // 如果最大数量为空，表示无上限
              insertSQL = "INSERT INTO quote_table (product_name, min_quantity, max_quantity, price) "
                         "VALUES (:productName, :minQuantity, NULL, :price)";
          } else {
              insertSQL = "INSERT INTO quote_table (product_name, min_quantity, max_quantity, price) "
                         "VALUES (:productName, :minQuantity, :maxQuantity, :price)";
          }

          // 使用预处理语句防止 SQL 注入
          query.prepare(insertSQL);
          query.bindValue(":productName", productName);
          query.bindValue(":minQuantity", minQuantity.toDouble());
          query.bindValue(":price", price.toDouble());

          if (!maxQuantity.isEmpty()) {
              query.bindValue(":maxQuantity", maxQuantity.toDouble());
          }

          // 执行插入操作
          bool ok = query.exec();
          if (ok) {
                 QMessageBox::information(this, "成功", "数据已成功存入数据库！");
                 clearInputs(); // 清空输入框
             } else {
                 QMessageBox::critical(this, "错误", "数据插入失败: " + query.lastError().text());
             }

}
void MainWindow::clearInputs()
{
    ui->name2->clear();
    ui->min->clear();
    ui->max->clear();
    ui->price->clear();
}




void MainWindow::on_input_clicked()
{
    QSqlDatabase db=QSqlDatabase::database("cc");
    QSqlQuery query(db);

    QString productName = ui->name->text();  // 产品名称
       QString weight = ui->weight->text();           // 重量

       // 验证输入
       if (productName.isEmpty() || weight.isEmpty()) {
           QMessageBox::warning(this, "输入错误", "请输入产品名称和重量！");
           return;
       }
       bool ok;
       double weightvalue = weight.toDouble(&ok);
          if (!ok) {
              QMessageBox::warning(this, "错误", "请输入有效的重量数值");
              return;
          }
          QString sql = "SELECT price FROM quote_table "
                         "WHERE product_name = :productName "
                         "AND min_quantity <= :weight "
                         "AND (max_quantity >= :weight OR max_quantity IS NULL) "
                         "ORDER BY min_quantity DESC "
                         "LIMIT 1";

            query.prepare(sql);
            query.bindValue(":productName", productName);
            query.bindValue(":weight", weightvalue);

            bool ok1 = query.exec();

            if (ok1 && query.next()) {
                // 查询成功，获取价格
                double price = query.value(0).toDouble();
//                QString message = QString("产品: %1\n重量: %2\n单价: %3 元")
//                                 .arg(productName)
//                                 .arg(weight)
//                                 .arg(price, 0, 'f', 2);
//                QMessageBox::information(this, "查询结果", message);

                double outout=price*weightvalue;
                ui->output->setText(QString::number(outout));


}}



void MainWindow::initTableView()
{
    // 创建数据模型
    QSqlTableModel *model = new QSqlTableModel(this, QSqlDatabase::database("cc"));
    model->setTable("quote_table");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 设置表头
    model->setHeaderData(model->fieldIndex("product_name"), Qt::Horizontal, tr("产品名称"));
    model->setHeaderData(model->fieldIndex("min_quantity"), Qt::Horizontal, tr("最小数量"));
    model->setHeaderData(model->fieldIndex("max_quantity"), Qt::Horizontal, tr("最大数量"));
    model->setHeaderData(model->fieldIndex("price"), Qt::Horizontal, tr("单价"));

    model->select();

    // 设置表格视图
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_show_clicked()
{
    initTableView(); // 刷新表格数据
        QMessageBox::information(this, "提示", "已刷新数据表");
}

void MainWindow::on_delete_2_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
      if (selection.isEmpty()) {
          QMessageBox::warning(this, "警告", "请先选择要删除的行");
          return;
      }

      QSqlDatabase db = QSqlDatabase::database("cc");
      QSqlQuery query(db);

      int row = selection.first().row();
      int id = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toInt();

      query.prepare("DELETE FROM quote_table WHERE id = :id");
      query.bindValue(":id", id);

      if (query.exec()) {
          QMessageBox::information(this, "成功", "数据删除成功");
          initTableView(); // 刷新表格视图
      } else {
          QMessageBox::critical(this, "错误", "删除失败: " + query.lastError().text());
      }

}
