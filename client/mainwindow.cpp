// Подключаем заголовочный файл нашего класса MainWindow
#include "mainwindow.h"
#include <QDebug>
// Подключаем сгенерированный заголовочный файл UI-формы
#include "./ui_mainwindow.h"
#include "authorization.h"
// Конструктор главного окна приложения
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)    // Инициализация базового класса QMainWindow
    , ui(new Ui::MainWindow) // Создание экземпляра UI-класса
{
    // Настройка пользовательского интерфейса
    ui->setupUi(this);
    task1 = false;
    task2 = false;
    result = false;
    ui->label_login->setText(authorization::getLogin());
}

// Деструктор главного окна
MainWindow::~MainWindow()
{
    // Освобождаем память, занятую UI-объектом
    delete ui;
}

void MainWindow::on_resultButton_clicked()
{
    if (result) {
        ui_result->show();
    }
    else{
        ui_result = new class result;
        result=true;
        ui_result->show();
    }
}

void MainWindow::on_task1Button_clicked(){
    if (task1) {
        ui_task1->show();
    }
    else{
        ui_task1 = new class task1;
        task1=true;
        ui_task1->show();
    }
}
void MainWindow::on_task2Button_clicked(){
    if (task2) {
        ui_task2->show();
    }
    else{
        ui_task2 = new class task2;
        task2=true;
        ui_task2->show();
    }

}
