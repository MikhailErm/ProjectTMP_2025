#include "mytcpserver.h"

// Подключаем обработчик команд (auth, register и т.д.)
#include "func2serv.h"

// Подключаем отладочный вывод и классы для работы с TCP
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>

//  Конструктор класса TCP-сервера
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    // Создаём экземпляр TCP-сервера
    mTcpServer = new QTcpServer(this);

    setClientStatesPointer(&clientStates);  // Мультиклиент

    // Подключаем сигнал "новое подключение" к слоту, который его обработает
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    // Пытаемся запустить сервер на порту 33333, принимая подключения с любых IP
    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server is not started"; // Если ошибка — выводим сообщение
    } else {
        qDebug() << "Server is started"; // Если всё хорошо — сообщаем, что сервер работает
    }
}

//  Деструктор TCP-сервера
MyTcpServer::~MyTcpServer()
{
    // При уничтожении объекта закрываем сервер
    mTcpServer->close();
}

//  Слот: обработка нового подключения клиента
void MyTcpServer::slotNewConnection()
{
    QTcpSocket *clientSocket = mTcpServer->nextPendingConnection(); // Мультиклиент
    int clientId = clientSocket->socketDescriptor();  // Мультиклиент

    clients[clientId] = clientSocket;                 // Мультиклиент
    clientStates[clientId] = ClientState();           // Мультиклиент

    socketToIdMap[clientSocket] = clientId;           // Сохраняем привязку сокета к clientId

    // Подключаем сигнал "данные пришли от клиента" к нашему слоту обработки данных
    connect(clientSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead); // Заменил на clientSocket (Мультиклиент)

    // Подключаем сигнал "клиент отключился" к слоту очистки
    connect(clientSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected); // Заменил на clientSocket (Мультиклиент)

    // Отправляем приветственное сообщение и инструкцию клиенту
    clientSocket->write("\nPlease enter command (e.g. register/auth&login&password):\r\n"); // Заменил на clientSocket (Мультиклиент)

    qDebug() << "Client connected:" << clientId;      // Мультиклиент
}

//  Слот: обработка входящих данных от клиента
void MyTcpServer::slotServerRead()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender()); // Мультиклиент
    if (!clientSocket) return; // Мультиклиент

    int clientId = clientSocket->socketDescriptor();  // Мультиклиент

    // Читаем все данные из сокета
    QByteArray buffer = clientSocket->readAll(); // Заменил на clientSocket (Мультиклиент)

    // Буферы для ответа (например, telnet-команды) и фактической команды пользователя
    QByteArray response;
    QByteArray actualMessage;

    // Обработка Telnet-команд
    // Telnet-протокол использует управляющие байты — нужно их отфильтровать
    int i = 0;
    while (i < buffer.size()) {
        if (static_cast<unsigned char>(buffer[i]) == 0xFF) { // 0xFF — начало telnet-команды
            if (i + 2 < buffer.size()) {
                char command = buffer[i + 1]; // команда (например, DO, DONT, WILL, WONT)
                char option = buffer[i + 2];  // опция

                // Ответы на команды клиента: DONT или WONT
                if (command == static_cast<char>(0xFB)) {
                    response.append("\xFF\xFC").append(option); // ответ DONT
                } else if (command == static_cast<char>(0xFD)) {
                    response.append("\xFF\xFE").append(option); // ответ WONT
                }

                i += 3; // пропускаем команду целиком (3 байта)
                continue;
            } else {
                break; // если команда неполная — прекращаем обработку
            }
        } else {
            // Если это обычный символ — добавляем его в команду
            actualMessage.append(buffer[i]);
            ++i;
        }
    }

    // Отправляем ответ клиенту (например, отказ от telnet-опций)
    if (!response.isEmpty()) {
        clientSocket->write(response); // Заменил на clientSocket (Мультиклиент)
    }

    // Переводим команду из байтов в строку и убираем пробелы/переводы строки
    QString decodedMessage = QString::fromUtf8(actualMessage).trimmed();

    // Выводим команду в отладочную консоль
    qDebug() << "Client" << clientId << " decoded message:" << decodedMessage; // Мультиклиент

    // Если строка пустая — ничего не делаем
    if (decodedMessage.isEmpty()) return;

    // Передаём команду в функцию parse() для обработки (auth, register и т.д.)
    QByteArray result = parsing(decodedMessage, clientId); // Мультиклиент , заменил на clientId

    // Отправляем результат клиенту + перевод строки
    clientSocket->write(result.append("\r\n")); // Заменил на clientSocket (Мультиклиент)
}

//  Слот: обработка отключения клиента (полностью переписан для мультиклиента)
void MyTcpServer::slotClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket) return;

    int clientId = socketToIdMap.value(clientSocket, -1);  // Мультиклиент

    qDebug() << "Client disconnected:" << clientId;

    clients.remove(clientId);                         // Мультиклиент
    clientStates.remove(clientId);                    // Мультиклиент
    socketToIdMap.remove(clientSocket);               // Мультиклиент

    clientSocket->deleteLater();
}
