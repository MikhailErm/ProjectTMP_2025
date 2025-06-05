#include <QtTest>
#include "func2serv.h"         // Функции сервера (auth, register, parsing)
#include "databasemanager.h"   // Менеджер базы данных
#include "gradientdescent.h"   // Градиентный спуск
#include <QSqlQuery>
#include <QMap>
#include <QDebug>

// Указатель на карту состояний клиентов — глобально
extern QMap<int, ClientState>* clientStatesPtr;

class TestParsing : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();             // Подготовка окружения
    void testAuth();                 // Тест авторизации
    void testRegister();             // Тест регистрации
    void testGradientQuadratic();   // Тест градиентного спуска
};

void TestParsing::initTestCase() {
    auto& db = DatabaseManager::instance().database();
    QVERIFY2(db.open(), "Failed to open database for testing.");

    QSqlQuery query(db);
    QVERIFY2(query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT, password TEXT)"),
             "Failed to create table users");
    QVERIFY2(query.exec("DELETE FROM users"), "Failed to clear users table");

    query.prepare("INSERT INTO users (login, password) VALUES (:login, :password)");
    query.bindValue(":login", "login");
    query.bindValue(":password", "pass");
    QVERIFY2(query.exec(), "Failed to insert test user");

    static QMap<int, ClientState> clientStates;
    clientStatesPtr = &clientStates;
    clientStates[1] = ClientState();
}

void TestParsing::testAuth() {
    QByteArray result = parsing("auth&testuser&1234", 1);
    qDebug() << "testAuth result:" << result;
    QVERIFY(result.contains("Authorization failed"));
}

void TestParsing::testRegister() {
    QByteArray result = parsing("register&testuser&1234", 1);
    QVERIFY(result.contains("registered") || result.contains("already exists"));
}

void TestParsing::testGradientQuadratic() {
    auto f = [](double x) { return (x - 3) * (x - 3); };
    auto df = [](double x) { return 2 * (x - 3); };

    QString result = gradientDescent(f, df, 0.0, 0.1, 100);
    qDebug() << "Gradient descent result:" << result;

    QVERIFY(result.contains("x ="));
    QVERIFY(result.contains("f(x) ="));
}

// Запускаем тестовый фреймворк Qt
QTEST_MAIN(TestParsing)
#include "test_parsing.moc"
