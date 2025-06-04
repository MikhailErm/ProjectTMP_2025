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
    QString answer = SingletonClient::getInstance()->send_msg_to_server("getTableData");
    //answer = "user1 f t|user2 t t|user3 f f";
    QStringList rowlist = answer.split("|");
    for (int i=0; i<rowlist.count(); i++){
        QStringList data = rowlist[i].split(" ");
        //QString login = data[0];
        QString task1;
        if (data[1]=="t") {task1="Correct";}
        else {task1="Wrong";}
        QString task2;
        if (data[2]=="t") {task2="Correct";}
        else {task2="Wrong";}

        int row = ui->resultTable->rowCount();
        ui->resultTable->insertRow(row);
        ui->resultTable->setItem(row, 0, new QTableWidgetItem(data[0])); //login
        ui->resultTable->setItem(row, 1, new QTableWidgetItem(task1)); //task1
        ui->resultTable->setItem(row, 2, new QTableWidgetItem(task2)); //task2
    }


}
