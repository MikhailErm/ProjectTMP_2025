#include "result.h"
#include "ui_result.h"
#include "singleton_client.h"
result::result(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::result)
{
    ui->setupUi(this);
}

result::~result()
{
    delete ui;
}

void result::on_displayResultButton_clicked(){
    /*for (int i = 0; i<=ui->resultTable->rowCount();i++){
        ui->resultTable->removeRow(i);
    }*/
    ui->resultTable->clearContents();
    ui->resultTable->setRowCount(0);
    QString answer = (SingletonClient::getInstance()->send_msg_to_server("get_all_stat"));
    //answer = "user1 f t|user2 t t|user3 f f";
    QStringList rowlist = answer.split("|");
    for (int i=0; i<rowlist.count()-1; i++){
        QStringList data = rowlist[i].split(" ");
        //QString login = data[0];
        qDebug()<<data[0];
        qDebug()<<data[1];
        qDebug()<<data[2];

       int row = ui->resultTable->rowCount();
        ui->resultTable->insertRow(row);
        ui->resultTable->setItem(row, 0, new QTableWidgetItem(data[0])); //login
        ui->resultTable->setItem(row, 1, new QTableWidgetItem(data[1])); //task1
        ui->resultTable->setItem(row, 2, new QTableWidgetItem(data[2])); //task2
    }


}
