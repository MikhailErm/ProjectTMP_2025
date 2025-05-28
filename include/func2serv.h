#ifndef FUNC2SERVER_H
#define FUNC2SERVER_H

#include <QString> // Мультиклиент
#include <QByteArray>
#include <QStringList>
#include <QMap> // Мультиклиент
#include "mytcpserver.h" // Мультиклиент

QByteArray parsing(QString msg, int sockId); // Добавли msg и sockId (Мультиклиент)
QByteArray auth(QStringList params, int sockId); //Добавил params и sockId (Мультиклиент)
QByteArray reg(QStringList params, int sockId); // Добавли sockId (Мультиклиент)

void setClientStatesPointer(QMap<int, ClientState> *states); // Мультиклиент

#endif // FUNC2SERV_H
