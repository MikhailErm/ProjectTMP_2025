// Подключаем заголовочный файл нашего класса MainWindow
#include "mainwindow.h"

// Подключаем сгенерированный заголовочный файл UI-формы
#include "./ui_mainwindow.h"

// Конструктор главного окна приложения
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)    // Инициализация базового класса QMainWindow
    , ui(new Ui::MainWindow) // Создание экземпляра UI-класса
{
    // Настройка пользовательского интерфейса
    ui->setupUi(this);
}

// Деструктор главного окна
MainWindow::~MainWindow()
{
    // Освобождаем память, занятую UI-объектом
    delete ui;
}
