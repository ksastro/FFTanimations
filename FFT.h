#pragma once
#include <iostream>
#include <vector>
#include <complex>

const double pi = std::acos(-1.0);
const double twopi = 2. * pi;

typedef std::vector<std::complex<double>> ComplexArray;

std::vector<int> ComplexToInt(ComplexArray input);
std::vector<std::vector<int>> ComplexToInt(std::vector<ComplexArray> input);

ComplexArray IntToComplex(std::vector<int> input);
std::vector<ComplexArray> IntToComplex(std::vector<std::vector<int>> input);


ComplexArray SortEvenOdd(ComplexArray input);
ComplexArray SortForFourier(ComplexArray input);


ComplexArray fft(ComplexArray& input);
std::vector<ComplexArray> fft(std::vector<ComplexArray>& input, int size);
std::vector<ComplexArray> fftFromInt(std::vector<std::vector<int>>& input, int size);


ComplexArray fftReverse(ComplexArray& input);
std::vector<ComplexArray> fftReverse(std::vector<ComplexArray>& input, int size);
std::vector<std::vector<int>> fftReverseToInt(std::vector<ComplexArray>& input, int size);


void fftUnitTest();
//void fft_base(ComplexArray& input);
//bool isPow2(int x);