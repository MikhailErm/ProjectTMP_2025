#include "gradientdescent.h"
#include <QRandomGenerator>
#include <QtMath>
#include <QDebug>
#include <limits>
#include <cmath>   // Для std::isnan
#include <functional> // Для std::function

namespace {
constexpr int MAX_ITERATIONS = 100000;
constexpr double LEARNING_RATE = 0.01;
constexpr double MOMENTUM = 0.2;
constexpr double EPSILON = 1e-8;
constexpr double INITIAL_GUESS = 1.0;

// Генерация случайного коэффициента от -5 до 5 включительно
int randomCoefficient() {
    return QRandomGenerator::global()->bounded(-5, 6);
}

// Генерируем случайный полином 4-й степени (коэффициенты a,b,c,d,e)
std::vector<int> generateValidPolynomial() {
    std::vector<int> coeffs(5);
    do {
        for(auto& c : coeffs) {
            c = randomCoefficient();
        }
    } while(coeffs[0] == 0); // Обеспечиваем, что старший коэффициент != 0
    return coeffs;
}

// Вычисление производной полинома в точке x
double calculateDerivative(double x, const std::vector<int>& coeffs) {
    return 4*coeffs[0]*x*x*x + 3*coeffs[1]*x*x + 2*coeffs[2]*x + coeffs[3];
}

// Вычисление значения полинома в точке x
double calculatePolynomialValue(double x, const std::vector<int>& coeffs) {
    // coeffs: a x^4 + b x^3 + c x^2 + d x + e
    return coeffs[0]*std::pow(x,4) + coeffs[1]*std::pow(x,3) + coeffs[2]*x*x + coeffs[3]*x + coeffs[4];
}

// Поиск минимума функции методом градиентного спуска с моментумом
// Возвращает округлённое до целого минимальное значение x, или NaN если минимум не найден
double findMinimum(const std::vector<int>& coeffs) {
    double x = INITIAL_GUESS;
    double velocity = 0.0;
    double prev_derivative = std::numeric_limits<double>::max();

    for(int i = 0; i < MAX_ITERATIONS; ++i) {
        double derivative = calculateDerivative(x, coeffs);

        // Критерий остановки: производная близка к 0 или не меняется
        if(std::abs(derivative) < EPSILON || std::abs(derivative - prev_derivative) < EPSILON) {
            break;
        }

        velocity = MOMENTUM * velocity + LEARNING_RATE * derivative;
        x -= velocity;
        prev_derivative = derivative;
    }

    // Проверяем, что в найденной точке минимум (вторая производная > 0)
    double second_derivative = 12*coeffs[0]*x*x + 6*coeffs[1]*x + 2*coeffs[2];
    if (second_derivative > 0) {
        return std::round(x); // Округляем до ближайшего целого
    } else {
        return std::numeric_limits<double>::quiet_NaN();
    }
}
} // namespace

// Форматируем полином для вывода
QString formatPolynomial(const std::vector<int>& coeffs) {
    QStringList terms;
    const QList<QString> powers = {"x^4", "x^3", "x^2", "x", ""};

    for(size_t i = 0; i < coeffs.size(); ++i) {
        if(coeffs[i] == 0) continue;

        QString term;
        if(!terms.empty() && coeffs[i] > 0) {
            term += "+";
        }
        term += QString::number(coeffs[i]);

        if(!powers[i].isEmpty()) {
            term += powers[i];
        }

        terms << term;
    }

    return terms.join(" ").replace("+", " + ").replace("-", " - ");
}

// Основная функция генерации задания с минимизацией
PolynomialTask findMinimumTask() {
    PolynomialTask task;
    task.valid = false;

    for(int attempt = 0; attempt < 100 && !task.valid; ++attempt) {
        auto coeffs = generateValidPolynomial();
        double minimum = findMinimum(coeffs);

        if(!std::isnan(minimum)) {
            task.equation = formatPolynomial(coeffs);
            task.minimum = minimum;
            task.valid = true;
            task.coefficients = coeffs;


            qDebug() << "[GradientDescent Task]";
            qDebug() << "Polynomial:" << task.equation;
            qDebug() << "Coefficients:" << coeffs;
            qDebug() << "Minimum (expected answer):" << task.minimum;
        }
    }

    if(!task.valid) {
        task.equation = "Не удалось сгенерировать подходящую функцию";
        task.minimum = 0.0;
    }

    return task;
}

// Реализация функции gradientDescent (если нужна)
// Пример простой реализации метода градиентного спуска без момента

QString gradientDescent(std::function<double(double)> f,
                        std::function<double(double)> df,
                        double initialX,
                        double learningRate,
                        int maxIterations)
{
    double x = initialX;
    double epsilon = 1e-8;
    double prev_x = x;

    for (int i = 0; i < maxIterations; ++i) {
        double grad = df(x);
        if (qAbs(grad) < epsilon) {
            break;
        }
        x = x - learningRate * grad;

        if (qAbs(x - prev_x) < epsilon) {
            break;
        }
        prev_x = x;
    }

    double value = f(x);
    return QString("x = %1").arg(x, 0, 'f', 6);
}
