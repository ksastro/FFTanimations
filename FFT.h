#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include "Image.h"


const double pi = std::acos(-1.0);
const double twopi = 2. * pi;

typedef std::vector<std::complex<double>> ComplexArray;
typedef std::vector<std::vector<std::complex<double>>> ComplexMatrix;

std::vector<int> ComplexToInt(ComplexArray input);
std::vector<std::vector<int>> ComplexToInt(std::vector<ComplexArray> input);
std::vector<std::vector<std::vector<int>>> ComplexToInt(std::vector<std::vector<ComplexArray>> input);

ComplexArray IntToComplex(std::vector<int> input);
std::vector<ComplexArray> IntToComplex(std::vector<std::vector<int>> input);
std::vector<std::vector<ComplexArray>> IntToComplex(std::vector<std::vector<std::vector<int>>> input);


ComplexArray SortEvenOdd(ComplexArray input);
ComplexArray SortForFourier(ComplexArray input);
ComplexMatrix TransposeSquareMatrix(ComplexMatrix input);


ComplexArray fft(ComplexArray& input, int mode);

ComplexArray fft(ComplexArray& input);
std::vector<ComplexArray> fft(std::vector<ComplexArray>& input, int size);
std::vector<ComplexArray> fftFromInt(std::vector<std::vector<int>>& input, int size);



ComplexArray fftReverse(ComplexArray& input);
std::vector<ComplexArray> fftReverse(std::vector<ComplexArray>& input, int size);
std::vector<std::vector<int>> fftReverseToInt(std::vector<ComplexArray>& input, int size);


ComplexMatrix fft2d(ComplexMatrix& input);
std::vector<ComplexMatrix> fft2d(std::vector<ComplexMatrix>& input);
ComplexMatrix fftReverse2d(ComplexMatrix& input);
std::vector<ComplexMatrix> fftReverse2d(std::vector<ComplexMatrix>& input);

void ExportFft2d(Image image, int size);


void fftUnitTest();
void fft2dUnitTest();
