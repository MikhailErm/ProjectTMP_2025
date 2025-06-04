#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <QVariantMap>
#include <vector>
#include <QString>
#include <functional>  // Добавлено для std::function

struct PolynomialTask {
    QString equation;               // Строка уравнения
    double minimum;                 // Найденный минимум
    bool valid;                    // Успешность
    std::vector<int> coefficients; // Коэффициенты (опционально)
};

PolynomialTask findMinimumTask();

QString formatPolynomial(const std::vector<int>& coeffs);
double calculatePolynomialValue(double x, const std::vector<int>& coeffs);

QString gradientDescent(std::function<double(double)> f,
                        std::function<double(double)> df,
                        double initialX,
                        double learningRate,
                        int maxIterations);

#endif // GRADIENTDESCENT_H
