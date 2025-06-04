#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHash>
#include <QMap>
#include "func2serv.h"  //  Используем уже определённую структуру ClientState

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
    QMap<int, QTcpSocket*> clients;           // Сокеты клиентов
    QMap<int, ClientState> clientStates;      // Состояние каждого клиента
    QHash<QTcpSocket*, int> socketToIdMap;    // Привязка сокета к ID
};

#endif // MYTCPSERVER_H
