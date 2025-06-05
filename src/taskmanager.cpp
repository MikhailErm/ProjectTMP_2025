#include "taskmanager.h"
#include <random>
#include <cmath>
#include "spline.h"
#include "gradientdescent.h"


TaskManager::TaskManager() {}

TaskData TaskManager::createTask(const QString& login, TaskType type) {
    TaskData task;
    switch (type) {
    case TaskType::GradientDescent:
        task = generateGradientDescentTask();
        break;
    case TaskType::Spline:
        task = generateSplineTask();
        break;
    }
    clientTasks_[login] = task;
    return task;
}

bool TaskManager::checkAnswer(const QString& login, QString answer) const {
    if (!clientTasks_.contains(login)) return false;

    bool okClient = false;
    double clientVal = answer.toDouble(&okClient);
    if (!okClient) return false;

    QString correctStr = clientTasks_.value(login).correctAnswer;
    bool okCorrect = false;
    double correctVal = correctStr.toDouble(&okCorrect);
    if (!okCorrect) return false;

    int clientInt = static_cast<int>(std::round(clientVal));
    int correctInt = static_cast<int>(std::round(correctVal));

    return (clientInt == correctInt);
}

TaskType TaskManager::getLastTaskType(const QString& login) const {
    if (!clientTasks_.contains(login)) return TaskType::Spline; // default
    return clientTasks_.value(login).type;
}


TaskData TaskManager::generateGradientDescentTask() {
    TaskData task;
    task.type = TaskType::GradientDescent;

    PolynomialTask polyTask = findMinimumTask(); // вызов твоей функции из gradientdescent.cpp

    if (polyTask.valid) {
        task.questionText = QString("Найдите минимум функции: %1").arg(polyTask.equation);
        task.correctAnswer = QString::number(static_cast<int>(std::round(polyTask.minimum)));
    } else {
        // В случае ошибки — дай запасной вопрос
        task.questionText = "Не удалось сгенерировать функцию для градиентного спуска.";
        task.correctAnswer = "";
    }

    return task;
}
TaskData TaskManager::generateSplineTask() {
    TaskData task;
    QString msg = runSpline();
    QStringList taskList = msg.split("|");
    task.correctAnswer= taskList[0];
    task.questionText = taskList[1]+taskList[2];

    return task;
}
