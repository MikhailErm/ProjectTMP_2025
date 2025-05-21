#ifndef MAINWINDOW_H  // Защита от повторного включения заголовочного файла
#define MAINWINDOW_H

#include <QMainWindow>  // Базовый класс для главного окна приложения

// Объявление пространства имен Ui, созданного Qt из .ui-файла
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;  // Предварительное объявление класса формы
}
QT_END_NAMESPACE

// Класс главного окна приложения, наследуется от QMainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT  // Макрос Qt, необходимый для работы сигналов и слотов

public:
    // Конструктор класса
    // Параметр:
    //   parent - указатель на родительский виджет (по умолчанию nullptr)
    MainWindow(QWidget *parent = nullptr);

    // Деструктор класса
    ~MainWindow();

private:
    Ui::MainWindow *ui;  // Указатель на сгенерированный интерфейс формы
};
#endif // MAINWINDOW_H  // Конец защиты от повторного включения
