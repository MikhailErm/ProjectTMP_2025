#ifndef RESULT_H
#define RESULT_H

#include <QWidget>

namespace Ui {
class result;
}

class result : public QWidget
{
    Q_OBJECT

public:
    explicit result(QWidget *parent = nullptr);
    ~result();

private slots:
    void on_displayResultButton_clicked();

private:
    Ui::result *ui;
};

#endif // RESULT_H
