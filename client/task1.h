#ifndef TASK1_H
#define TASK1_H

#include <QWidget>

namespace Ui {
class task1;
}

class task1 : public QWidget
{
    Q_OBJECT

public:
    explicit task1(QWidget *parent = nullptr);
    ~task1();
    void getTask();
    bool isStarted();
    bool isCompleted();

private slots:
    void on_answerButton_clicked(); //answerButton
private:
    Ui::task1 *ui;
    bool completed = false;
    bool started = false;
};

#endif // TASK1_H
