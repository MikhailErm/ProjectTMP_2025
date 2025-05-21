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
    // Получаем сокет нового клиента
    mTcpSocket = mTcpServer->nextPendingConnection();

    // Подключаем сигнал "данные пришли от клиента" к нашему слоту обработки данных
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);

    // Подключаем сигнал "клиент отключился" к слоту очистки
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    // Отправляем приветственное сообщение и инструкцию клиенту
    mTcpSocket->write("\nPlease enter command (e.g. auth&login&password):\r\n");
}

// ===== Слот: обработка входящих данных от клиента =====
void MyTcpServer::slotServerRead()
{
    // Читаем все данные из сокета
    QByteArray buffer = mTcpSocket->readAll();

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
        mTcpSocket->write(response);
    }

    // Переводим команду из байтов в строку и убираем пробелы/переводы строки
    QString decodedMessage = QString::fromUtf8(actualMessage).trimmed();

    // Выводим команду в отладочную консоль
    qDebug() << "Decoded message:" << decodedMessage;

    // Если строка пустая — ничего не делаем
    if (decodedMessage.isEmpty()) return;

    // Передаём команду в функцию parse() для обработки (auth, register и т.д.)
    QByteArray result = parse(decodedMessage, mTcpSocket->socketDescriptor());

    // Отправляем результат клиенту + перевод строки
    mTcpSocket->write(result.append("\r\n"));
}

//  Слот: обработка отключения клиента
void MyTcpServer::slotClientDisconnected()
{
    // Закрываем соединение, когда клиент отключается
    mTcpSocket->close();
}
