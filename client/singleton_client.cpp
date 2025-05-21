// Подключаем заголовочный файл класса
#include "singleton_client.h"
// Подключаем необходимые Qt-компоненты
#include "QMessageBox"
#include <QCoreApplication>

// Инициализация статических членов класса
Singleton_client* Singleton_client::pInstance = nullptr;  // Указатель на единственный экземпляр
QTcpSocket* Singleton_client::tcpInstance = nullptr;      // Указатель на TCP-сокет
Singleton_clientDestroyer Singleton_client::destroyer;     // Объект-разрушитель

// Конструктор класса (protected, чтобы нельзя было создать экземпляр напрямую)
Singleton_client::Singleton_client()
{
    this->initTCP();  // Инициализируем TCP-соединение при создании экземпляра
}

// Деструктор класса
Singleton_client::~Singleton_client()
{
    // Закрываем TCP-соединение перед уничтожением объекта
    this->tcpInstance->close();
}

// Метод инициализации TCP-соединения
void Singleton_client::initTCP() {
    // Создаем новый TCP-сокет (родитель - текущий объект)
    this->tcpInstance = new QTcpSocket(this);

    // Пытаемся подключиться к серверу (localhost, порт 33333)
    this->tcpInstance->connectToHost("127.0.0.1", 33333);

    // Ждем подключения в течение 1 секунды
    if (this->tcpInstance->waitForConnected(1000)) {
        qDebug() << "Connected!";  // Успешное подключение

        // Ждем доступных данных для чтения (1 секунда)
        tcpInstance->waitForReadyRead(1000);
        // Читаем все доступные данные (очищаем буфер)
        this->tcpInstance->readAll();
    }
    else {
        qDebug() << "Not connected!";  // Подключение не удалось
        // Можно добавить обработку ошибки подключения
    }
}

// Метод для выполнения запроса к серверу
QString Singleton_client::doRequest(QByteArray request) {
    // Отправляем запрос серверу
    this->tcpInstance->write(request);

    // Ожидаем отправки данных (1 секунда)
    this->tcpInstance->waitForBytesWritten(1000);

    // Ожидаем ответа от сервера (2 секунды)
    this->tcpInstance->waitForReadyRead(2000);

    // Читаем весь ответ от сервера
    QByteArray response = this->tcpInstance->readAll();

    // Преобразуем ответ в QString и возвращаем
    return QString::fromUtf8(response);
}
