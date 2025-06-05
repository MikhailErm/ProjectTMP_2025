#include "databasemanager.h"

// Подключаем классы Qt для выполнения SQL-запросов и отладки
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

//  Конструктор DatabaseManager
DatabaseManager::DatabaseManager() {
    // Указываем, что будем использовать SQLite как драйвер базы данных
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

//  Деструктор DatabaseManager
DatabaseManager::~DatabaseManager() {
    // Если база открыта — закрываем соединение
    if (m_db.isOpen()) {
        m_db.close();
    }
}

//  Реализация паттерна "синглтон"
// Возвращает ссылку на единственный экземпляр DatabaseManager
DatabaseManager& DatabaseManager::instance() {
    // static — гарантирует, что объект создаётся один раз
    static DatabaseManager instance;
    return instance;
}

//  Подключение к SQLite-базе данных
bool DatabaseManager::connect(const QString& dbPath) {
    // Устанавливаем путь к базе данных (имя файла .db)
    m_db.setDatabaseName(dbPath);

    // Пытаемся открыть базу
    if (!m_db.open()) {
        // В случае ошибки выводим сообщение в консоль
        qDebug() << "Failed to connect to database:" << m_db.lastError().text();
        return false; // Сообщаем, что подключение не удалось
    }

    return true; // Подключение успешно
}

//  Доступ к объекту базы данных
QSqlDatabase& DatabaseManager::database() {
    return m_db; // Возвращаем ссылку на базу данных
}

//  Инициализация базы (создание таблицы users)
bool DatabaseManager::initialize() {
    // Создаём SQL-запрос в текущей базе
    QSqlQuery query(m_db);

    // Выполняем SQL-команду для создания таблицы users, если она ещё не существует
    return query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT, -- автоинкрементный ID
            login TEXT UNIQUE NOT NULL,            -- уникальный логин
            password TEXT NOT NULL                 -- пароль (пока в открытом виде)
        )
    )");

    // Возвращает true, если таблица успешно создана или уже существует
}


