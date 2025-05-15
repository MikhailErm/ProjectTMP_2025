#include "func2serv.h"
#include <QStringList>
#include <QByteArray>


QByteArray auth(QStringList params) {
    if (params.size() < 2) {
        return QByteArray("Error: missing login or password");
    }

    QString login = params[0];
    QString password = params[1];

    if (login == "Admin" && password == "1234") {
        return QByteArray("Login successful! You can now use the echo server.");
    } else {
        return QByteArray("Authorization failed. Try again.");
    }
}


QByteArray parse(QString msg, int sockId)
{
    QStringList params = msg.split("&");
    QString func = params[0];
    params.pop_front();

    if (func == "auth") {
        return auth(params);
    }

    return QByteArray("Unknown function");
}
