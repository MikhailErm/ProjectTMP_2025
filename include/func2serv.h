#ifndef FUNC2SERVER_H
#define FUNC2SERVER_H

#include <QString> // Мультиклиент
#include <QByteArray>
#include <QStringList>
#include <QMap> // Мультиклиент


QByteArray parsing(QString msg, int sockId); // Добавли msg и sockId (Мультиклиент)
QByteArray auth(QStringList params, int sockId); //Добавил params и sockId (Мультиклиент)
QByteArray reg(QStringList params, int sockId); // Добавли sockId (Мультиклиент)


struct ClientState {
    bool isAuthorized = false;
    QString login;
    bool task1Correct = false;
    bool task2Correct = false;
};
extern QMap<int, ClientState>* clientStatesPtr;
void setClientStatesPointer(QMap<int, ClientState> *states); // Мультиклиент


QString all_stat();

#endif // FUNC2SERV_H
