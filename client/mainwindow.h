#ifndef MAINWINDOW_H // Защита от повторного включения заголовочного файла
#define MAINWINDOW_H

#include <QMainWindow> // Базовый класс для главного окна приложения
#include "task1.h"
#include "task2.h"
#include "result.h"
// Объявление пространства имен Ui, созданного Qt из .ui-файла
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow; // Предварительное объявление класса формы
}
QT_END_NAMESPACE

// Класс главного окна приложения, наследуется от QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT // Макрос Qt, необходимый для работы сигналов и слотов

public:
    // Конструктор класса
    // Параметр:
    //   parent - указатель на родительский виджет (по умолчанию nullptr)
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_resultButton_clicked();
    void on_task1Button_clicked();
    void on_task2Button_clicked();

private:
    Ui::MainWindow *ui;
    task1 *ui_task1;
    task2 *ui_task2;
    result *ui_result;
    bool task1;
    bool task2;
    bool result;

    //task2 *ui_task2;    // Указатель на сгенерированный интерфейс формы
};
#endif // MAINWINDOW_H  // Конец защиты от повторного включения
