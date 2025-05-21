#include "func2serv.h"
#include "databasemanager.h"

#include <QStringList>
#include <QByteArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

// ===== Авторизация =====
QByteArray auth(QStringList params) {
    if (params.size() < 2) {
        return QByteArray("Error: missing login or password");
    }

    QString login = params[0];
    QString password = params[1];

    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (query.exec() && query.next()) {
        return QByteArray("Login successful! You can now use the echo server.");
    } else {
        return QByteArray("Authorization failed. Try again.");
    }
}

// ===== Регистрация =====
QByteArray registerUser(QStringList params) {
    if (params.size() < 2) {
        return QByteArray("Error: missing login or password");
    }

    QString login = params[0];
    QString password = params[1];

    // Проверка: существует ли логин
    QSqlQuery checkQuery(DatabaseManager::instance().database());
    checkQuery.prepare("SELECT * FROM users WHERE login = :login");
    checkQuery.bindValue(":login", login);
    if (checkQuery.exec() && checkQuery.next()) {
        return QByteArray("Error: user already exists");
    }

    // Вставка нового пользователя
    QSqlQuery insertQuery(DatabaseManager::instance().database());
    insertQuery.prepare("INSERT INTO users (login, password) VALUES (:login, :password)");
    insertQuery.bindValue(":login", login);
    insertQuery.bindValue(":password", password);

    if (!insertQuery.exec()) {
        return QByteArray("Error: failed to register user");
    }

    return QByteArray("User registered successfully. You can now login.");
}

// ===== Обработка команд =====
QByteArray parse(QString msg, int sockId)
{
    QStringList params = msg.split("&");
    QString func = params[0];
    params.pop_front();

    if (func == "auth") {
        return auth(params);
    } else if (func == "register") {
        return registerUser(params);
    }

    return QByteArray("Unknown function");
}
