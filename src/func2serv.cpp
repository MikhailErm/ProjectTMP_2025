#include "func2serv.h"
#include "databasemanager.h"
#include "taskmanager.h"
#include "RSA.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

QMap<int, ClientState> *clientStatesPtr = nullptr;
TaskManager taskManager;

void setClientStatesPointer(QMap<int, ClientState> *states) {
    clientStatesPtr = states;
}

QByteArray reg(QStringList params, int sockId) {
    if (params.size() < 3) return "Usage: register&login&password";

    QString login = QString::fromStdString(RSA::decrypt(params[1].toStdString(), "64507:41473"));
    QString password = params[2];

    auto& db = DatabaseManager::instance();
    if (!db.database().isOpen()) return "Database not connected";

    QSqlQuery query(db.database());
    query.prepare("INSERT INTO users (login, password) VALUES (?, ?)");
    query.addBindValue(login);
    query.addBindValue(password);

    if (!query.exec()) {
        QString err = query.lastError().text();
        if (err.contains("UNIQUE constraint failed"))
            return "User already exists";
        else
            return "Registration failed: " + err.toUtf8();
    }

    qDebug() << "User registered:" << login;
    return "Registration successful";
}

QByteArray auth(QStringList params, int sockId) {
    if (params.size() < 3) return "Usage: auth&login&password";

    try {
        QString login = QString::fromStdString(RSA::decrypt(params[1].toStdString(), "64507:41473"));
        QString password = params[2];

        auto& db = DatabaseManager::instance();
        if (!db.database().isOpen()) return "Database not connected";

        QSqlQuery query(db.database());
        query.prepare("SELECT password FROM users WHERE login = ?");
        query.addBindValue(login);

        if (!query.exec() || !query.next()) return "User not found";

        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            if (clientStatesPtr) {
                (*clientStatesPtr)[sockId].isAuthorized = true;
                (*clientStatesPtr)[sockId].login = login;
            }
            return "Authentication successful";
        }
        return "Wrong password";
    } catch (...) {
        return "Decryption error";
    }
}

QString all_stat() {
    if (!clientStatesPtr) return QString("No data");

    QString result;
    for (auto it = clientStatesPtr->begin(); it != clientStatesPtr->end(); ++it) {
        const ClientState &state = it.value();

        QString ans1 = state.task1Correct ? "correct" : "wrong";
        QString ans2 = state.task2Correct ? "correct" : "wrong";

        result += state.login + " " + ans1 + " " + ans2 + "|";
    }

    return result;
}

bool checkTask1Answer(const QVector<double> &answers) {
    static const QVector<double> correctAnswers = {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0,
        10.0, 11.0, 12.0
    };

    if (answers.size() != correctAnswers.size())
        return false;

    const double EPS = 1e-5;
    for (int i = 0; i < answers.size(); ++i) {
        if (qAbs(answers[i] - correctAnswers[i]) > EPS)
            return false;
    }
    return true;
}

QByteArray parsing(QString msg, int sockId) {
    // Исправленная строка для Qt5:
    QStringList parts = msg.split('&', Qt::SkipEmptyParts);
    
    if (parts.isEmpty()) return "Invalid command";

    QString cmd = parts[0].toLower();

    if (cmd == "register") return reg(parts, sockId);
    if (cmd == "auth") return auth(parts, sockId);

    if (!clientStatesPtr || !clientStatesPtr->contains(sockId))
        return "Unknown client";
    if (!(*clientStatesPtr)[sockId].isAuthorized)
        return "Unauthorized. Please authenticate first.";

    QString login = (*clientStatesPtr)[sockId].login;

    if (cmd == "get_task1") {
        auto task = taskManager.createTask(login, TaskType::Spline);
        return task.questionText.toUtf8();
    } else if (cmd == "get_task2") {
        auto task = taskManager.createTask(login, TaskType::GradientDescent);
        return task.questionText.toUtf8();
    } else if (cmd == "answer") {
        if (parts.size() == 13) {
            QVector<double> answers;
            bool allOk = true;
            for (int i = 1; i <= 12; ++i) {
                bool ok = false;
                double val = parts[i].toDouble(&ok);
                if (!ok) {
                    allOk = false;
                    break;
                }
                answers.append(val);
            }
            if (!allOk) return "Invalid number format in answers";

            bool correct = checkTask1Answer(answers);
            auto& client = (*clientStatesPtr)[sockId];
            client.task1Correct = correct;

            return correct ? "Correct answer!" : "Incorrect answer, try again.";
        }
        else if (parts.size() == 2) {
            bool ok = false;
            double numAnswer = parts[1].trimmed().toDouble(&ok);
            if (!ok) return "Invalid answer format";
            QString answer = QString::number(numAnswer, 'f', 5);

            auto& client = (*clientStatesPtr)[sockId];
            bool correct = taskManager.checkAnswer(client.login, answer);

            if (taskManager.getLastTaskType(client.login) == TaskType::Spline)
                client.task1Correct = correct;
            else if (taskManager.getLastTaskType(client.login) == TaskType::GradientDescent)
                client.task2Correct = correct;

            return correct ? "Correct answer!" : "Incorrect answer, try again.";
        }
        else {
            return "Invalid answer format";
        }
    } else if (cmd == "status") {
        const auto& client = (*clientStatesPtr)[sockId];
        return QString("Status for %1: Task1: %2 Task2: %3")
            .arg(client.login)
            .arg(client.task1Correct ? "t" : "f")
            .arg(client.task2Correct ? "t" : "f")
            .toUtf8();
    } else if (cmd == "get_all_stat") {
        return all_stat().toUtf8();
    }

    return "Unknown command";
}
