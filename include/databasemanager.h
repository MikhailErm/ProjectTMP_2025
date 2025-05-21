#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

// Подключаем модуль Qt для работы с базой данных
#include <QSqlDatabase>

// Класс DatabaseManager реализует доступ к базе данных как синглтон
class DatabaseManager
{
public:
    // Получить ссылку на единственный экземпляр класса (синглтон)
    static DatabaseManager& instance();

    // Получить ссылку на объект базы данных (для выполнения SQL-запросов)
    QSqlDatabase& database();

    // Подключение к базе данных по пути (имя файла SQLite)
    bool connect(const QString& dbPath);

    // Инициализация базы данных: создание таблицы users (если она не существует)
    bool initialize();

private:
    // Приватный конструктор — нельзя создать объект напрямую
    DatabaseManager();

    // Приватный деструктор
    ~DatabaseManager();

    // Удаляем конструктор копирования (нельзя копировать экземпляр синглтона)
    DatabaseManager(const DatabaseManager&) = delete;

    // Удаляем оператор присваивания
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // Объект базы данных (член класса)
    QSqlDatabase m_db;
};

// Завершение защиты от повторного включения
#endif
