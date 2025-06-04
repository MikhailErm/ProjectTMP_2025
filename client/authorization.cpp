// authorization.cpp
#include "authorization.h"
#include "ui_authorization.h"
#include "myCrypt.h"
#include <QMessageBox>
#include "singleton_client.h"
// Конструктор класса формы авторизации
authorization::authorization(QWidget *parent)
    : QWidget(parent)                // Инициализация базового класса
    , ui(new Ui::authorization)      // Создание UI-интерфейса
    , ui_main(nullptr)               // Инициализация указателя на главное окно
{
    ui->setupUi(this);  // Настройка пользовательского интерфейса
    // Скрываем элементы, связанные с регистрацией (по умолчанию форма авторизации)
    ui->regButton->setVisible(false);

}
QString authorization::usingLogin = "";

// Деструктор класса
authorization::~authorization()
{
    delete ui;        // Освобождаем память от UI
    delete ui_main;   // Освобождаем память от главного окна
}
QString authorization::getLogin() {
    return usingLogin;
}
// Метод проверки авторизации (заглушка)
bool authorization::is_auth(const QString &login, const QString &password)
{
    QString msg = "auth&" + login + "&" + password;
    QString answer;
    answer = SingletonClient::getInstance()->send_msg_to_server(msg).trimmed();
    if (answer=="Authentication successful") {
        return true;
    }
    else return false;
}

// Метод проверки регистрации (заглушка)
bool authorization::is_reg(const QString &login, const QString &password)
{
    QString msg = "register&" + login + "&" + password;
    QString answer;
    answer = SingletonClient::getInstance()->send_msg_to_server(msg).trimmed();
    if (answer=="Registration successful") {
        return true;
    }
    else return false;

}

// Слот обработки нажатия кнопки авторизации
void authorization::on_loginButton_clicked()
{
    // Получаем введенные данные, удаляя пробелы по краям
    QString login = ui->lineEdit_login->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();
    std::string loginStd = login.toStdString();
    std::string encrypted = SHA256::RSA::encrypt(loginStd);
    login = QString::fromStdString(encrypted);
    password = SHA256::sha256(password);

    // Проверка на пустые поля
    if(login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Логин и пароль не могут быть пустыми");
        return;
    }

    // Проверка авторизации
    bool check = is_auth(login, password);
    if (check) {
        usingLogin=login;
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
void authorization::on_changeButton_clicked()
{
    // Определяем текущее состояние (видимость кнопки регистрации)
    bool flag = ui->regButton->isVisible();

    // Переключаем видимость элементов:
    ui->loginButton->setVisible(flag);  // Кнопка авторизации
    ui->regButton->setVisible(!flag); // Кнопка регистрации
}

// Слот обработки нажатия кнопки регистрации
void authorization::on_regButton_clicked()
{
    // Получаем введенные данные
    QString login = ui->lineEdit_login->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();
    std::string loginStd = login.toStdString();
    std::string encrypted = SHA256::RSA::encrypt(loginStd);
    login = QString::fromStdString(encrypted);
    password = SHA256::sha256(password);
    // Проверка на заполненность всех полей
    if(login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
        return;
    }

    // Попытка регистрации
    bool check = is_reg(login, password);
    if (check) {
        QMessageBox::information(this, "Регистрация", "Регистрация прошла успешно, авторизуйтесь.");
    }
    else {
        QMessageBox::warning(this, "Ошибка", "Регистрация не удалась.");
    }
}
