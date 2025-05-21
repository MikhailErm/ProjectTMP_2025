#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connect(const QString& dbPath) {
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qDebug() << "Failed to connect to database:" << m_db.lastError().text();
        return false;
    }
    return true;
}

QSqlDatabase& DatabaseManager::database() {
    return m_db;
}

bool DatabaseManager::initialize() {
    QSqlQuery query(m_db);
    return query.exec(R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            login TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL
        )
    )");
}
