#include <QCoreApplication>
#include "mytcpserver.h"
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Подключение к SQLite базе данных
    if (!DatabaseManager::instance().connect("users.db")) {
        qDebug() << "Database connection failed. Exiting.";
        return -1;
    }

    // Создание таблицы users, если её нет
    if (!DatabaseManager::instance().initialize()) {
        qDebug() << "Failed to initialize database.";
        return -2;
    }


    MyTcpServer myserv;

    return a.exec();
}
