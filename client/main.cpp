// Подключаем заголовочный файл нашей формы авторизации
#include "authorization.h"
#include <QDebug>
// Подключаем основной заголовочный файл Qt для приложений
#include <QApplication>

// Точка входа в приложение
int main(int argc, char *argv[])
{
    // Создаем объект приложения Qt
    // argc - количество аргументов командной строки
    // argv - массив строк аргументов командной строки
    QApplication a(argc, argv);

    // Создаем экземпляр нашей формы авторизации
    authorization w;

    // Показываем форму авторизации
    w.show();
    // Получаем экземпляр SingletonClient
    SingletonClient::getInstance();

    // Пример отправки сообщения на сервер
    std::string msg;
    msg="hello";
    qDebug()<< SingletonClient::getInstance()->send_msg_to_server(QString::fromStdString(msg));
    // Запускаем главный цикл обработки событий приложения
    // Этот вызов блокирует выполнение до закрытия приложения
    return a.exec();
}
