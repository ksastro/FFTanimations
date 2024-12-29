#include "Flattener.h"
#include "Image.h"
#include <vector>


static int bitConvertXYtoL(int XY) {		//input = xy;
	int output = 0;
	switch (XY) {
	case (0 + 0):
		return (0 + 0);
	case (0 + 1):
		return (0 + 1);
	case (2 + 0):
		return (2 + 1);
	case (2 + 1):
		return (2 + 0);
	}
	throw std::runtime_error("input for curveBit is not 0-3 integer");
}

static int bitConvertLtoXY(int L) {
	return bitConvertXYtoL(L);		//yeah, the transform is symmetric
}


static ivec2 Rotate(int xyMax, ivec2 xyTail, int xyBits) {
	ivec2 output = xyTail;
	if ((xyBits & 1) == 0) {
		if ((xyBits >> 1) == 1) {
			xyTail.x = xyMax - xyTail.x;
			xyTail.y = xyMax - xyTail.y;
		}
		output.x = xyTail.y;
		output.y = xyTail.x;
	}
	return output;
}

int HilbertCurve(ivec2 xy, int bitSize) {
	if (xy.x > 1 << bitSize || xy.y > 1 << bitSize) {
		throw std::runtime_error("input is out of bounds");
		return -1;
	}
	int output, xMasked, yMasked, xyMasked, mask;
	mask = 1 << (bitSize - 1);
	output = 0;
	int previousXY = 1;
	//xy = RotateIfNeeded(xy, bitSize);
	for (int i = bitSize - 1; i >= 0; i--) {
		xMasked = (xy.x & mask) << 1;
		yMasked = (xy.y & mask);
		xyMasked = (xMasked + yMasked) >> i;
		int L = bitConvertXYtoL(xyMasked);
		//L = MirrorIfNeeded(L, previousXY);
		previousXY = xyMasked;	//for the next cycle
		L = L << (2 * i);
		output += L;
		mask >> 1;
	}
	return output;
}

ivec2 HilbertCurve(int l, int bitSize) {
	ivec2 output = { 0, 0 };
	int previousXY = 1;
	

	for (int i = 0; i < bitSize; i++) {
		int mask = (2 + 1) << (2 * i);
		int LMasked = (l & mask) >> (2 * i);

		int xyMasked = bitConvertXYtoL(LMasked);
		int xyMax = (1 << i) - 1;

		output = Rotate(xyMax, output, xyMasked);
		int xMasked = (xyMasked >> 1) << i;
		int yMasked = (xyMasked & 1) << i;	
		output.x += xMasked;
		output.y += yMasked;
	}
	//output = RotateIfNeeded(output, bitSize);
	return output;
}

std::vector<Color> HilbertCurve(std::vector<std::vector<Color>> input2d, int bitSize) {
	std::vector<Color> output;
	for (int i = 0; i < 1 << (2 * bitSize); i++) {
		ivec2 ij = HilbertCurve(i, bitSize);
		output.push_back(input2d[ij.x][ij.y]);
	}
	return output;
}
std::vector<int> HilbertCurve(std::vector<std::vector<int>> input2d, int bitSize) {
	std::vector<int> output;
	for (int i = 0; i < 1 << (2 * bitSize); i++) {
		ivec2 ij = HilbertCurve(i, bitSize);
		output.push_back(input2d[ij.x][ij.y]);
	}
	return output;
}
std::vector<ivec2> HilbertCurve(std::vector<std::vector<ivec2>> input2d, int bitSize) {
	std::vector<ivec2> output;
	for (int i = 0; i < 1 << (2 * bitSize); i++) {
		ivec2 ij = HilbertCurve(i, bitSize);
		output.push_back(input2d[ij.x][ij.y]);
	}
	return output;
}
std::vector<Color> HilbertCurve(Image image, int bitSize) {
	std::vector<Color> output;
	for (int i = 0; i < 1 << (2 * bitSize); i++) {
		ivec2 ij = HilbertCurve(i, bitSize);
		output.push_back(image.GetColor(ij.x, ij.y));
	}
	return output;
}


std::vector<std::vector<int>> HilbertReverse(std::vector<int> curve, int bitSize) 
{
	int size = 1 << bitSize;
	std::vector<std::vector<int>> output(size, std::vector<int>(size));
	for (int i = 0; i < size * size; i++) {
		ivec2 xy = HilbertCurve(i, bitSize);
		output[xy.x][xy.y] = curve[i];
	}
	return output;
}
std::vector<std::vector<std::vector<int>>> HilbertReverse(std::vector<std::vector<int>> curves, int bitSize)
{
	std::vector<std::vector<std::vector<int>>> output;
	for (std::vector<int> element : curves) {
		output.push_back(HilbertReverse(element, bitSize));
	}
	return output;
}

void unitTestHilbert() {
	std::vector<std::vector<ivec2>> input;
	const int bitSize = 9;
	int size = 1 << bitSize;
	for (int i = 0; i < size; i++) {
		input.push_back({});
		for (int j = 0; j < size; j++) {
			input[i].push_back({ i,j });
		}
	}

	std::vector<ivec2> hilbertCurve = HilbertCurve(input, bitSize);
	std::cout << std::size(hilbertCurve) << std::endl;
	std::cout << "{";
	for (int i = 0; i < size * size; i++) {
		std::cout << "{" << hilbertCurve[i].x << ", " << hilbertCurve[i].y << "}, ";
	}
	std::cout << "}" << std::endl;
}