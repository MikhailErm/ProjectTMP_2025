#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

// Подключаем базовый класс Qt (нужен для сигналов и слотов)
#include <QObject>

// Подключаем класс TCP-сервера (принимает входящие соединения)
#include <QTcpServer>

// Подключаем класс TCP-сокета (отвечает за связь с клиентом)
#include <QTcpSocket>

#include <QMap>

// Структура состояния клиента
struct ClientState { // Мультиклиент
    bool isAuthorized = false;
    int loginAttempt = 0;
    QString login;
};

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer *mTcpServer;
    QMap<int, QTcpSocket*> clients;           // (Мультиклиент) Сокеты клиентов по дескриптору
    QMap<int, ClientState> clientStates;      // (Мультиклиент) Состояние каждого клиента
};

#endif // MYTCPSERVER_H






/*// Определяем класс TCP-сервера, наследующийся от QObject
class MyTcpServer : public QObject
{
    Q_OBJECT  // Макрос Qt — включает поддержку сигналов и слотов

public:
    // Конструктор с необязательным родителем (nullptr по умолчанию)
    explicit MyTcpServer(QObject *parent = nullptr);

    // Деструктор — освобождает ресурсы
    ~MyTcpServer();

private slots:
    // Слот вызывается при новом входящем соединении
    void slotNewConnection();

    // Слот вызывается, когда клиент отключается
    void slotClientDisconnected();

    // Слот вызывается, когда клиент присылает данные
    void slotServerRead();

private:
    // Указатель на сервер (ждёт подключений)
    QTcpServer *mTcpServer;

    // Указатель на сокет клиента (только один клиент одновременно в текущей реализации)
    QTcpSocket *mTcpSocket;

private:
    // Флаг: авторизован ли текущий клиент
    bool isAuthorized = false;

    // Вспомогательные переменные для возможной пошаговой авторизации или состояний
    QString currentStep;       // Текущий этап взаимодействия (пока не используется)
    QString loginAttempt;      // Попытка логина
    QString passwordAttempt;   // Попытка пароля
};

// Завершаем защиту от повторного включения
#endif */
