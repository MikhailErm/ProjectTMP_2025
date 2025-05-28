#include <QtTest>
#include "func2serv.h"       // Заголовок с функциями сервера (auth, register, parsing)
#include "databasemanager.h" // Менеджер базы данных
#include <QSqlQuery>
#include <QMap>

// Указатель на карту состояний клиентов — объявляется extern, чтобы использовать глобально
extern QMap<int, ClientState>* clientStatesPtr;

class TestParsing : public QObject {
    Q_OBJECT

private slots:
    // Этот слот вызывается один раз перед запуском всех тестов — инициализация окружения
    void initTestCase();

    // Тестируем функцию авторизации
    void testAuth();

    // Тестируем функцию регистрации
    void testRegister();
};

void TestParsing::initTestCase() {
    // Получаем ссылку на объект базы данных из менеджера
    auto& db = DatabaseManager::instance().database();

    // Открываем базу данных, проверяем, что открытие прошло успешно
    QVERIFY2(db.open(), "Failed to open database for testing.");

    QSqlQuery query(db);

    // Создаём таблицу пользователей, если её ещё нет
    QVERIFY2(query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT, password TEXT)"),
             "Failed to create table users");

    // Очищаем таблицу пользователей, чтобы тесты стартовали с чистого листа
    QVERIFY2(query.exec("DELETE FROM users"), "Failed to clear users table");

    // Вставляем одного тестового пользователя с логином "login" и паролем "pass"
    query.prepare("INSERT INTO users (login, password) VALUES (:login, :password)");
    query.bindValue(":login", "login");
    query.bindValue(":password", "pass");
    QVERIFY2(query.exec(), "Failed to insert test user");

    // Инициализируем карту состояний клиентов, которую использует сервер
    static QMap<int, ClientState> clientStates;
    clientStatesPtr = &clientStates;

    // Создаём состояние для клиента с сокет ID=1 (неавторизован по умолчанию)
    clientStates[1] = ClientState();
}

void TestParsing::testAuth() {
    // Пытаемся авторизоваться под несуществующим пользователем
    QByteArray result = parsing("auth&testuser&1234", 1);

    // Выводим результат для отладки в консоль
    qDebug() << "testAuth result:" << result;

    // Проверяем, что ответ содержит сообщение об ошибке авторизации
    QVERIFY(result.contains("Authorization failed"));
}

void TestParsing::testRegister() {
    // Пытаемся зарегистрировать пользователя "testuser" с паролем "1234"
    QByteArray result = parsing("register&testuser&1234", 1);

    // Проверяем, что ответ либо подтверждает регистрацию, либо говорит, что пользователь уже существует
    QVERIFY(result.contains("registered") || result.contains("already exists"));
}

// Запускаем тестовый фреймворк Qt
QTEST_MAIN(TestParsing)

// Подключаем MOC (Meta-Object Compiler) для генерации дополнительного кода Qt
#include "test_parsing.moc"
