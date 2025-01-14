#include "FFT.h"


static bool isPow2(int x) {
	return (x > 0 && (x & (x - 1)) == 0);
};

std::vector<int> ComplexToInt(ComplexArray input)
{

	std::vector<int> output;
	
	for (std::complex<double> number : input) {
		double magn = std::abs(number);
		if (magn > 255) magn = 255;
		output.push_back((int)std::round(magn));
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
std::vector<std::vector<std::vector<int>>> ComplexToInt(std::vector<std::vector<ComplexArray>> input)
{
	std::vector<std::vector<std::vector<int>>> output;
	for (std::vector<ComplexArray> element : input) {
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
std::vector<std::vector<ComplexArray>> IntToComplex(std::vector<std::vector<std::vector<int>>> input)
{
	std::vector<std::vector<ComplexArray>> output;
	for (std::vector<std::vector<int>> element : input) {
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

ComplexMatrix TransposeSquareMatrix(ComplexMatrix input)
{
	int size = (int)input.size();
	ComplexMatrix output(size, std::vector<std::complex<double>>(size));
	if (input[0].size() != size)
	{
		std::cout << "matrix is not square!";
	}
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			output[x][y] = input[y][x];
		}
	}
	return output;
}


ComplexArray fft(ComplexArray& input, int mode) {
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


static ComplexMatrix fft2d(ComplexMatrix& input, int mode)
{
	
	int size = (int)input.size();

	// Create output vector with appropriate size
	ComplexMatrix oneDimFFT(size, std::vector<std::complex<double>>(size));
	ComplexMatrix output(size, std::vector<std::complex<double>>(size));

	// Apply FFT along each row
	for (int i = 0; i < size; i++) {
		oneDimFFT[i] = fft(input[i], mode);
	}

	oneDimFFT = TransposeSquareMatrix(oneDimFFT);

	// Apply FFT along each column
	for (int i = 0; i < size; i++) {
		output[i] = fft(oneDimFFT[i], mode);
	}

	output = TransposeSquareMatrix(output);

	return output;
}

ComplexMatrix fft2d(ComplexMatrix& input)
{
	return fft2d(input, 1);
}
std::vector<ComplexMatrix> fft2d(std::vector<ComplexMatrix>& input)
{
	int size = (int)std::size(input);
	std::vector<std::vector<ComplexArray>> output;
	for (int i = 0; i < size; i++) {
		output.push_back(fft2d(input[i]));
	}
	return output;
}

ComplexMatrix fftReverse2d(ComplexMatrix& input)
{
	return fft2d(input, -1);
}
std::vector<ComplexMatrix> fftReverse2d(std::vector<ComplexMatrix>& input)
{
	int size = (int)std::size(input);
	std::vector<std::vector<ComplexArray>> output;
	for (int i = 0; i < size; i++) {
		output.push_back(fftReverse2d(input[i]));
	}
	return output;
}

void ExportFft2d(Image image, int size)
{
	std::cout << "Starting color extraction" << std::endl;
	std::vector<std::vector<std::vector<int>>> ColorsSeparated;
	image.ExtractColorArraysTo(ColorsSeparated);
	std::cout << "Color extraction finished" << std::endl << std::endl;

	std::cout << "Starting comlex converting" << std::endl;
	std::vector<std::vector<ComplexArray>> ColorsComplex = IntToComplex(ColorsSeparated);
	std::cout << "Comlex converting finished" << std::endl << std::endl;

	std::cout << "Starting fft" << std::endl;
	std::vector<ComplexMatrix> fftsByColor = fft2d(ColorsComplex);
	std::cout << "Fft finished" << std::endl << std::endl;

	std::vector<std::vector<std::vector<Color>>> fftsAsColorMatrices;

	for (int colorID = 0; colorID < 3; colorID++) {
		fftsAsColorMatrices.push_back({});
		for (int row = 0; row < size; row++) {
			fftsAsColorMatrices[colorID].push_back({});
			for (int column = 0; column < size; column++) {
				double magnitude = std::abs(fftsByColor[colorID][row][column]);
				double phase = std::arg(fftsByColor[colorID][row][column]);
				Color color = Color::ConvertComplexToColor(magnitude, phase);
				fftsAsColorMatrices[colorID][row].push_back(color);
			}
		}
	}

	std::vector<Image> fftsAsFrames;
	for (int i = 0; i < std::size(fftsAsColorMatrices); i++) {
		fftsAsFrames.push_back(Image(size, size, fftsAsColorMatrices[i]));
		std::string name = std::to_string(i) + ".bmp";
		fftsAsFrames[i].Export(name);
	}

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

void fft2dUnitTest() {
	int size = 4;
	ComplexMatrix input = { {0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15} };
	//ComplexMatrix input = { {0,1},{2,3}};
	ComplexMatrix Fourier = fft2d(input);
	std::cout << "Fourier transform result:" << std::endl;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			std::cout << Fourier[x][y] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	
	ComplexMatrix Reverse = fftReverse2d(Fourier);
	std::vector<std::vector<int>> output = ComplexToInt(Reverse);
	std::cout << "Inverse Fourier transform result:" << std::endl;
	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			std::cout << output[x][y] << ", ";
		}
		std::cout << std::endl;
	}
}