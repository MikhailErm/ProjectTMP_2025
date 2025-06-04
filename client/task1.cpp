#include "task1.h"
#include "singleton_client.h"
#include "ui_task1.h"
#include "authorization.h"
#include <QDebug>
#include <QMessageBox>
task1::task1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::task1)
{
    ui->setupUi(this);
}

task1::~task1()
{
    delete ui;
}

void task1::on_answerButton_clicked()
{
    double a0, a1, a2, b0, b1, b2, c0, c1, c2, d0, d1, d2;

    a0=ui->a0->text().trimmed().toDouble();
    a1=ui->a1->text().trimmed().toDouble();
    a2=ui->a2->text().trimmed().toDouble();
    b0=ui->b0->text().trimmed().toDouble();
    b1=ui->b1->text().trimmed().toDouble();
    b2=ui->b2->text().trimmed().toDouble();
    c0=ui->c0->text().trimmed().toDouble();
    c1=ui->c1->text().trimmed().toDouble();
    c2=ui->c2->text().trimmed().toDouble();
    d0=ui->d0->text().trimmed().toDouble();
    d1=ui->d1->text().trimmed().toDouble();
    d2=ui->d2->text().trimmed().toDouble();

    QString msg = authorization::getLogin() + " " +
                QString::number(a0)+" "+QString::number(a1) +" "+QString::number(a2)
                  +" "+QString::number(b0)+" "+QString::number(b1)+" "+QString::number(b2)
                  +" "+QString::number(c0)+" "+QString::number(c1)+" "+QString::number(c2)
                  +" "+QString::number(d0)+" "+QString::number(d1)+" "+QString::number(d2);

    SingletonClient::getInstance()->send_msg_to_server(msg);

    ui->a0->setReadOnly(true);
    ui->a1->setReadOnly(true);
    ui->a2->setReadOnly(true);
    ui->b0->setReadOnly(true);
    ui->b1->setReadOnly(true);
    ui->b2->setReadOnly(true);
    ui->c1->setReadOnly(true);
    ui->c2->setReadOnly(true);
    ui->c0->setReadOnly(true);
    ui->d0->setReadOnly(true);
    ui->d1->setReadOnly(true);
    ui->d2->setReadOnly(true);
    ui->answerButton->setEnabled(false);

    QMessageBox::information(this, "Задача закончена", "Ответы отправлены.");

    this->close();


}
