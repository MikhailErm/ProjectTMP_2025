// authorization.cpp
#include "authorization.h"
#include "ui_authorization.h"
#include <QMessageBox>

// Конструктор класса формы авторизации
authorization::authorization(QWidget *parent)
    : QWidget(parent)                // Инициализация базового класса
    , ui(new Ui::authorization)      // Создание UI-интерфейса
    , ui_main(nullptr)               // Инициализация указателя на главное окно
{
    ui->setupUi(this);  // Настройка пользовательского интерфейса

    // Скрываем элементы, связанные с регистрацией (по умолчанию форма авторизации)
    ui->pushButton_registration->setVisible(false);
    ui->label_email->setVisible(false);
    ui->lineEdit_email->setVisible(false);
}

// Деструктор класса
authorization::~authorization()
{
    delete ui;        // Освобождаем память от UI
    delete ui_main;   // Освобождаем память от главного окна
}

// Метод проверки авторизации (заглушка)
bool authorization::is_auth(const QString &login, const QString &password)
{
    // TODO: Заменить на реальную проверку через БД или сервер
    // Временная проверка - поля не должны быть пустыми
    return !login.isEmpty() && !password.isEmpty();
}

// Метод проверки регистрации (заглушка)
bool authorization::is_reg(const QString &email, const QString &login, const QString &password)
{
    // TODO: Заменить на реальную проверку через БД или сервер
    // Временная проверка - все поля не должны быть пустыми
    return !email.isEmpty() && !login.isEmpty() && !password.isEmpty();
}

// Слот обработки нажатия кнопки авторизации
void authorization::on_pushButton_authorization_clicked()
{
    // Получаем введенные данные, удаляя пробелы по краям
    QString login = ui->lineEdit_login->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    // Проверка на пустые поля
    if(login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми");
        return;
    }

    // Проверка авторизации
    bool check = is_auth(login, password);
    if (check) {
        QMessageBox::information(this, "Авторизация", "Авторизация прошла успешно.");
        this->close();  // Закрываем окно авторизации

        // Создаем и показываем главное окно приложения
        ui_main = new MainWindow;
        ui_main->show();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Введен неправильный логин или пароль.");
    }
}

// Слот обработки нажатия кнопки смены режима (авторизация/регистрация)
void authorization::on_pushButton_change_clicked()
{
    // Определяем текущее состояние (видимость кнопки регистрации)
    bool flag = ui->pushButton_registration->isVisible();

    // Переключаем видимость элементов:
    ui->pushButton_authorization->setVisible(flag);  // Кнопка авторизации
    ui->label_email->setVisible(!flag);             // Метка для email
    ui->lineEdit_email->setVisible(!flag);          // Поле ввода email
    ui->pushButton_registration->setVisible(!flag); // Кнопка регистрации
}

// Слот обработки нажатия кнопки регистрации
void authorization::on_pushButton_registration_clicked()
{
    // Получаем введенные данные
    QString email = ui->lineEdit_email->text().trimmed();
    QString login = ui->lineEdit_login->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    // Проверка на заполненность всех полей
    if(email.isEmpty() || login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
        return;
    }

    // Попытка регистрации
    bool check = is_reg(email, login, password);
    if (check) {
        QMessageBox::information(this, "Регистрация", "Регистрация прошла успешно.");
        this->close();  // Закрываем окно регистрации

        // Создаем и показываем главное окно приложения
        ui_main = new MainWindow;
        ui_main->show();
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Регистрация не удалась.");
    }
}
