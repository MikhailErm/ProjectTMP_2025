#ifndef FUNC2SERVER_H
#define FUNC2SERVER_H

#include <QByteArray>
#include <QStringList>

QByteArray parsing(QString, int);
QByteArray auth(QStringList);
QByteArray reg(QStringList params);
QByteArray lookallstat(QStringList value);

#endif
