#ifndef FUNCTIONCLIENT_H
#define FUNCTIONCLIENT_H

// Подключение заголовочного файла с реализацией клиента-синглтона
#include "singleton_client.h"

#include <QObject>  // Базовый класс Qt для работы с сигналами/слотами

bool result_graf_min();

bool result_halving_method(QString, QString, QString, QString);

#endif
