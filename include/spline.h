#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <QString>
int RandomInt1(int min, int max);

std::vector<int> rand_x();
std::vector<int> rand_y();

std::vector<int> assign_a(const std::vector<int>& y);
std::vector<int> assign_h(std::vector<int>& x);

std::vector<std::vector<double>> matrix_A(std::vector<int>& h);
std::vector<std::vector<double>> matrix_B(std::vector<int>& y, std::vector<int>& h);

double det(const std::vector<std::vector<double>>& a, int n);
double con(const std::vector<std::vector<double>>& a, int n, int h, int c);
std::vector<std::vector<double>> nghichdao(const std::vector<std::vector<double>>& a, int n);

std::vector<double> tichMaTran(const std::vector<std::vector<double>>& a, const std::vector<std::vector<double>>& b, int m, int n, int m1);

std::vector<double> assign_b(std::vector<int>& y, std::vector<int>& h, std::vector<double>& c);
std::vector<double> assign_d(std::vector<int>& h, std::vector<double>& c);

QString topic(std::vector<int>& x, std::vector<int>& y);

// Wrapper function to execute the spline generation
QString runSpline();

#endif // SPLINE_H
