#include "task2.h"
#include "ui_task2.h"
#include <QMessageBox>
#include "authorization.h"
#include "singleton_client.h"
task2::task2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::task2)
{
    ui->setupUi(this);
    QString function = SingletonClient::getInstance()->send_msg_to_server("get_task2");
    ui->label_function_data->setText(function);
}

task2::~task2()
{
    delete ui;
}
void task2::on_answerButton_clicked(){

    double x=ui->input_x->text().trimmed().toDouble();

    QString msg = "answer&" + QString::number(x);

    ui->input_x->setReadOnly(true);
    ui->answerButton->setEnabled(false);

    QMessageBox::information(this, "Задача закончена", "Ответы отправлены.");

    this->close();
}
