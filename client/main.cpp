// Подключаем заголовочный файл нашей формы авторизации
#include "authorization.h"
#include <QDebug>
// Подключаем основной заголовочный файл Qt для приложений
#include <QApplication>
#include "task1.h"
#include "singleton_client.h"
// Точка входа в приложение
int main(int argc, char *argv[])
{
    // Создаем объект приложения Qt
    // argc - количество аргументов командной строки
    // argv - массив строк аргументов командной строки
    QApplication a(argc, argv);

    // Создаем экземпляр нашей формы авторизации
    authorization w;
    task1 w1;

    // Показываем форму авторизации
    w.show();
    // Получаем экземпляр SingletonClient
    SingletonClient::getInstance();

    return a.exec();
}
