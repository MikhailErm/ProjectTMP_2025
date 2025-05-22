#include "func2serv.h"
#include "databasemanager.h"

// Подключаем стандартные Qt-модули для работы со строками и SQL
#include <QStringList>
#include <QByteArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

//  Авторизация
// Функция для проверки логина и пароля пользователя
QByteArray auth(QStringList params) {
    // Проверка, что пришло хотя бы два параметра (логин и пароль)
    if (params.size() < 2) {
        return QByteArray("Error: missing login or password"); // Недостаточно аргументов
    }

    // Извлекаем логин и пароль
    QString login = params[0];
    QString password = params[1];

    // Создаём SQL-запрос на выборку пользователя с заданным логином и паролем
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    // Если запрос выполнен и найдена запись — авторизация успешна
    if (query.exec() && query.next()) {
        return QByteArray("Login successful! You can now use the echo server.");
    } else {
        // Иначе — ошибка авторизации
        return QByteArray("Authorization failed. Try again.");
    }
}

//  Регистрация
// Функция для регистрации нового пользователя
QByteArray reg(QStringList params) {
    // Проверка на наличие логина и пароля
    if (params.size() < 2) {
        return QByteArray("Error: missing login or password");
    }

    // Сохраняем логин и пароль из аргументов
    QString login = params[0];
    QString password = params[1];

    // Проверка: существует ли пользователь с таким логином
    QSqlQuery checkQuery(DatabaseManager::instance().database());
    checkQuery.prepare("SELECT * FROM users WHERE login = :login");
    checkQuery.bindValue(":login", login);
    if (checkQuery.exec() && checkQuery.next()) {
        // Если такой логин уже есть — возвращаем ошибку
        return QByteArray("Error: user already exists");
    }

    // Вставка нового пользователя в таблицу
    QSqlQuery insertQuery(DatabaseManager::instance().database());
    insertQuery.prepare("INSERT INTO users (login, password) VALUES (:login, :password)");
    insertQuery.bindValue(":login", login);
    insertQuery.bindValue(":password", password);

    // Проверка, успешно ли выполнен запрос
    if (!insertQuery.exec()) {
        return QByteArray("Error: failed to register user");
    }

    // Успешная регистрация
    return QByteArray("User registered successfully. You can now login.");
}

// Обработка команд от клиента
// Главная функция обработки текста команды
QByteArray parsing(QString msg, int sockId)
{
    // Разбиваем строку на параметры по символу "&"
    QStringList params = msg.split("&");

    // Первый элемент — имя команды (auth, register и т.д.)
    QString func = params[0];

    // Удаляем команду из списка, остаются только параметры
    params.pop_front();

    // Выбор нужной функции по имени команды
    if (func == "auth") {
        return auth(params); // авторизация
    } else if (func == "register") {
        return reg(params); // регистрация
    }

    // Если команда не распознана — возвращаем ошибку
    return QByteArray("Unknown function");
}
