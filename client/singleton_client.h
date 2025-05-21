#ifndef SINGLETON_CLIENT_H
#define SINGLETON_CLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

// Предварительное объявление класса Singleton_client для использования в Singleton_clientDestroyer
class Singleton_client;

// Класс-разрушитель для корректного освобождения памяти синглтона
class Singleton_clientDestroyer
{
private:
    Singleton_client* pInstance;  // Указатель на экземпляр синглтона
public:
    ~Singleton_clientDestroyer() {
        delete this->pInstance;  // Освобождение памяти при уничтожении разрушителя
    }
    void initialize(Singleton_client * p) {
        this->pInstance = p;  // Инициализация указателя на синглтон
    }
};

// Класс Singleton_client - реализация клиента как синглтона
class Singleton_client: public QObject {
private:
    static Singleton_client* pInstance;  // Статический указатель на единственный экземпляр
    void initTCP();  // Приватный метод для инициализации TCP соединения
    static QTcpSocket* tcpInstance;  // Статический указатель на TCP сокет
    static Singleton_clientDestroyer destroyer;  // Статический экземпляр разрушителя

protected:
    Singleton_client();  // Защищенный конструктор
    Singleton_client(const Singleton_client&) = delete;  // Запрет копирования
    Singleton_client& operator = (Singleton_client &) = delete;  // Запрет присваивания
    ~Singleton_client();  // Защищенный деструктор
    friend class Singleton_clientDestroyer;  // Разрешаем разрушителю доступ к приватным членам

public:
    // Статический метод для получения экземпляра синглтона
    static Singleton_client* getInstance() {
        if (!pInstance) {
            pInstance = new Singleton_client();  // Создаем экземпляр, если его нет
            destroyer.initialize(pInstance);     // Инициализируем разрушитель
        }
        return pInstance;
    }

    // Метод для выполнения запроса к серверу
    QString doRequest(QByteArray request);
};

#endif // Singleton_client_H
