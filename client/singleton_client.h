#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>

class SingletonClient;

class SingletonDestroyer {
private:
    SingletonClient* p_instance;
public:
    ~SingletonDestroyer() { delete p_instance; }
    void initialize(SingletonClient* p) { p_instance = p; }
};

class SingletonClient : public QObject {
    Q_OBJECT
private:
    static SingletonClient* p_instance;
    static SingletonDestroyer destroyer;
    QTcpSocket* mTcpSocket;
protected:
    explicit SingletonClient(QObject *parent = nullptr);
    SingletonClient(SingletonClient&) = delete;
    ~SingletonClient();
    friend class SingletonDestroyer;
public:
    static SingletonClient* getInstance();
    QString send_msg_to_server(QString query);
};

#endif // SINGLETON_H
