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
    completed=false;
    started=false;

}

task2::~task2()
{
    delete ui;
}
bool task2::isCompleted(){
    return completed;
}
bool task2::isStarted(){
    return started;
}
void task2::getTask(){
    if (!completed) {
        started=true;
        QString function = (SingletonClient::getInstance()->send_msg_to_server("get_task2") + " : ").split(": ")[1];
        ui->label_function_data->setText(function);
    }
}
void task2::on_answerButton_clicked(){

    double x=ui->input_x->text().trimmed().toDouble();

    QString msg = "answer&" + QString::number(x);

    ui->input_x->setReadOnly(true);
    ui->answerButton->setEnabled(false);

    QMessageBox::information(this, "Задача закончена", "Ответы отправлены.");
    completed=true;
    this->close();
}
