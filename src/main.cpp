#include <QCoreApplication>

// Подключаем наш TCP-сервер
#include "mytcpserver.h"

// Подключаем менеджер базы данных (синглтон)
#include "databasemanager.h"

int main(int argc, char *argv[])
{
    // Создаём Qt-приложение
    QCoreApplication a(argc, argv);

    //  Подключение к базе данных SQLite
    // Пытаемся подключиться к файлу базы данных "users.db"
    // Если не удалось подключиться, выводим сообщение и выходим с кодом -1
    if (!DatabaseManager::instance().connect("users.db")) {
        qDebug() << "Database connection failed. Exiting.";
        return -1;
    }

    //  Инициализация базы данных
    // Создаём таблицу users (если она ещё не существует)
    // Если создание не удалось — выводим сообщение и завершаем программу с кодом -2
    if (!DatabaseManager::instance().initialize()) {
        qDebug() << "Failed to initialize database.";
        return -2;
    }

    // Запуск TCP-сервера
    // Создаём объект нашего сервера, который начнёт слушать порт (33333)
    MyTcpServer myserv;

    // Запускаем основной цикл приложения Qt
    return a.exec();
}
