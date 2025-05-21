#include "mytcpserver.h"
#include "func2serv.h"

#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Server is not started";
    } else {
        qDebug() << "Server is started";
    }
}

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    mTcpSocket->write("\nPlease enter command (e.g. auth&login&password):\r\n");
}

void MyTcpServer::slotServerRead()
{
    QByteArray buffer = mTcpSocket->readAll();
    QByteArray response;
    QByteArray actualMessage;


    int i = 0;
    while (i < buffer.size()) {
        if (static_cast<unsigned char>(buffer[i]) == 0xFF) {
            if (i + 2 < buffer.size()) {
                char command = buffer[i + 1];
                char option = buffer[i + 2];
                if (command == static_cast<char>(0xFB)) {
                    response.append("\xFF\xFC").append(option);
                } else if (command == static_cast<char>(0xFD)) {
                    response.append("\xFF\xFE").append(option);
                }
                i += 3;
                continue;
            } else {
                break;
            }
        } else {
            actualMessage.append(buffer[i]);
            ++i;
        }
    }

    if (!response.isEmpty()) {
        mTcpSocket->write(response);
    }

    QString decodedMessage = QString::fromUtf8(actualMessage).trimmed();
    qDebug() << "Decoded message:" << decodedMessage;

    if (decodedMessage.isEmpty()) return;


    QByteArray result = parse(decodedMessage, mTcpSocket->socketDescriptor());
    mTcpSocket->write(result.append("\r\n"));
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}
