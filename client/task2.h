#ifndef TASK2_H
#define TASK2_H

#include <QWidget>

namespace Ui {
class task2;
}

class task2 : public QWidget
{
    Q_OBJECT

public:
    explicit task2(QWidget *parent = nullptr);
    ~task2();
    void getTask();
    bool isStarted();
    bool isCompleted();

private slots:
    void on_answerButton_clicked(); //answerButton

private:
    Ui::task2 *ui;
    bool completed=false;
    bool started=false;
};

#endif // TASK2_H
