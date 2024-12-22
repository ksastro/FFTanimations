#pragma once
#include <iostream>
#include <vector>
#include <complex>

const double pi = std::acos(-1.0);
const double twopi = 2. * pi;

typedef std::vector<std::complex<double>> ComplexArray;
ComplexArray SortEvenOdd(ComplexArray input);
ComplexArray fft(ComplexArray& input);
ComplexArray SortForFourier(ComplexArray input);
//void fft_base(ComplexArray& input);
//bool isPow2(int x);