#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QString>
#include <QMap>

enum class TaskType {
    GradientDescent,
    Spline
};

struct TaskData {
    TaskType type;
    QString questionText;
    QString correctAnswer;
};

class TaskManager {
public:
    TaskManager();
    TaskData createTask(const QString& login, TaskType type);
    bool checkAnswer(const QString& login, QString answer) const;
    TaskType getLastTaskType(const QString& login) const;

private:
    TaskData generateGradientDescentTask();
    TaskData generateSplineTask();

    QMap<QString, TaskData> clientTasks_;
};

#endif // TASKMANAGER_H
