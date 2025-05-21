#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

// Подключение необходимых заголовочных файлов
#include <QWidget>          // Базовый класс для виджетов Qt
#include <QMessageBox>      // Для отображения сообщений пользователю
#include "mainwindow.h"     // Заголовочный файл главного окна приложения
#include "functionClient.h" // Заголовочный файл с функциями клиента

// Пространство имен для UI-класса, сгенерированного из .ui-файла
namespace Ui {
class authorization;
}

// Класс формы авторизации, наследуется от QWidget
class authorization : public QWidget
{
    Q_OBJECT  // Макрос Qt для поддержки сигналов и слотов

public:
    explicit authorization(QWidget *parent = nullptr);  // Конструктор
    ~authorization();                                   // Деструктор

private slots:
    // Слоты - обработчики событий кнопок:
    void on_pushButton_authorization_clicked();  // Клик по кнопке авторизации
    void on_pushButton_change_clicked();         // Клик по кнопке смены (пароля?)
    void on_pushButton_registration_clicked();   // Клик по кнопке регистрации

private:
    Ui::authorization *ui;      // Указатель на сгенерированный UI-класс
    MainWindow *ui_main;         // Указатель на главное окно приложения

private:
    // Приватные методы для проверок:
    bool is_auth(const QString &login, const QString &password);  // Проверка авторизации
    bool is_reg(const QString &email, const QString &login,
                const QString &password);  // Проверка регистрации
};

#endif // AUTHORIZATION_H
