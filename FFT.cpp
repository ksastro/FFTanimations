#include "FFT.h"
#include "image.h"
#include <complex>
#include <vector>

static bool isPow2(int x) {
	return (x > 0 && (x & (x - 1)) == 0);
};

std::vector<int> ComplexToInt(ComplexArray input)
{
	std::vector<int> output;
	for (std::complex<double> number : input) {
		output.push_back((int)std::abs(number));
	}
	return output;
}

std::vector<std::vector<int>> ComplexToInt(std::vector<ComplexArray> input)
{
	std::vector<std::vector<int>> output;
	for (ComplexArray element : input) {
		output.push_back(ComplexToInt(element));
	}
	return output;
}

ComplexArray IntToComplex(std::vector<int> input)
{
	ComplexArray output;
	for (int number : input) {
		output.push_back((std::complex<double>)number);
	}
	return output;
}

std::vector<ComplexArray> IntToComplex(std::vector<std::vector<int>> input) 
{
	std::vector<ComplexArray> output;
	for (std::vector<int> element : input) {
		output.push_back(IntToComplex(element));
	}
	return output;
}

ComplexArray SortEvenOdd(ComplexArray input) {
	int size = (int)std::size(input);
	std::vector<std::complex<double>> output;
	for (int i = 0; i < size / 2; i++) {
		output.push_back(input[2 * i]);
	}
	for (int i = 0; i < size / 2; i++) {
		output.push_back(input[2 * i + 1]);
	}
	return output;
}

static int BitMirror(int x, int bitSize) {
	int y = 0;
	int temp;
	for (int i = 0; i < bitSize; i++) {
		temp = 1 << i;
		temp = temp & x;
		temp = (temp > 0);
		temp = temp << (bitSize - i - 1);
		y += temp;
	}
	return y;
};

ComplexArray SortForFourier(ComplexArray input) {
	std::vector<std::complex<double>> output;
	std::complex<double> take;
	int mirrorIndex = 0;
	int size = (int)std::size(input);
	for (int i = 0; i < size; i++) {
		mirrorIndex = BitMirror(i, (int)log2(size));
		take = input[mirrorIndex];
		output.push_back(take);
	}
	return output;
}

static ComplexArray fft(ComplexArray& input, int mode) {
	int size = (int)std::size(input);
	double multiplier = 1 / sqrt(double(size));
	if (!isPow2(size)) {
		throw std::runtime_error("input size must be a power of 2");
		return input;
	}
	if (mode != 1 && mode != -1) {
		throw std::runtime_error("FFT mode must be either 1 or -1");
		return input;
	}
	std::vector<std::complex<double>> output = SortForFourier(input);
	std::complex<double> even, odd, w;
	int evenIndex, oddIndex, clusterSize;
	for (int level = 0; level < log2(size); level++) {
	/*for (int level = (int)log2(size) - 1; level >= 0 ; level--) {*/
		clusterSize = (1 << level);			//(1 << level) = 2 ^ level
		for (int cluster = 0; cluster < size /(2 * clusterSize); cluster++) {
			for (int offset = 0; offset < clusterSize; offset++) {
				w = std::polar(1., mode * 2 * pi * offset / (2 * clusterSize));
				evenIndex = offset + (2 * clusterSize) * cluster;
				oddIndex = evenIndex + clusterSize;
				even = output[evenIndex];
				odd = output[oddIndex];
				output[evenIndex] = even + w * odd;
				output[oddIndex] = even - w * odd;
			}
		}
	}
	for (int i = 0; i < size; i++) {
		output[i] *= multiplier;
	}
	return output;
}

ComplexArray fft(ComplexArray& input) {
	return fft(input, 1);
}
std::vector<ComplexArray> fft(std::vector<ComplexArray>& input, int size)
{
	std::vector<ComplexArray> output;
	for (int i = 0; i < size; i++) {
		output.push_back(fft(input[i]));
	}
	return output;
}
std::vector<ComplexArray> fftFromInt(std::vector<std::vector<int>>& input, int size)
{
	std::vector<ComplexArray> converted = IntToComplex(input);
	return fft(converted, size);
}

ComplexArray fftReverse(ComplexArray& input) {
	return fft(input, -1);
}
std::vector<ComplexArray> fftReverse(std::vector<ComplexArray>& input, int size)
{
	std::vector<ComplexArray> output;
	for (int i = 0; i < size; i++) {
		output.push_back(fftReverse(input[i]));
	}
	return output;
}
std::vector<std::vector<int>> fftReverseToInt(std::vector<ComplexArray>& input, int size)
{
	return ComplexToInt(fftReverse(input, size));
}


void fftUnitTest() {
	ComplexArray input = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	ComplexArray Fourier = fft(input);
	std::cout << "Fourier transform result:" << std::endl;
	for (std::complex<double> number : Fourier) {
		std::cout << number << std::endl;
	}
	std::cout <<  std::endl;
	ComplexArray Reverse = fftReverse(Fourier);
	std::vector<int> output = ComplexToInt(Reverse);
	std::cout << "Inverse Fourier transform result:" << std::endl;
	for (auto number : output) {
		std::cout << number << std::endl;
	}
}