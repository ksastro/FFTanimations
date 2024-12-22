#include "FFT.h"
#include <complex>

static bool isPow2(int x) {
	return (x > 0 && (x & (x - 1)) == 0);
};

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



ComplexArray fft(ComplexArray& input) {
	int size = (int)std::size(input);
	if (!isPow2(size)) {
		throw std::runtime_error("input size must be a power of 2");
		return input;
	}
	std::vector<std::complex<double>> output = SortForFourier(input);
	std::complex<double> even, odd, w;
	int evenIndex, oddIndex, clusterSize;
	for (int level = 0; level < log2(size); level++) {
	/*for (int level = (int)log2(size) - 1; level >= 0 ; level--) {*/
		for (std::complex<double> number : output) {
			std::cout << number;
		}
		std::cout << std::endl;
		clusterSize = (1 << level);			//(1 << level) = 2 ^ level
		for (int cluster = 0; cluster < size /(2 * clusterSize); cluster++) {
			for (int offset = 0; offset < clusterSize; offset++) {
				w = std::polar(1., 2 * pi * offset / (2 * clusterSize));
				evenIndex = offset + (2 * clusterSize) * cluster;
				oddIndex = evenIndex + clusterSize;
				even = output[evenIndex];
				odd = output[oddIndex];
				output[evenIndex] = even + w * odd;
				output[oddIndex] = even - w * odd;
			}
		}
	}
	for (std::complex<double> number : output) {
		std::cout << number;
	}
	return output;
}

//void fft(ComplexArray& input) {
//	try { fft_base(input); }
//	catch (const std::exception& exception) {
//		std::cerr << "Error: " << exception.what() << std::endl;
//	}
//}


