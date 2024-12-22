#include "Flattener.h"
#include <vector>

static int bitConvertXYtoL(int XY) {		//input = xy;
	int output = 0;
	switch (XY) {
	case (0 + 0):
		return 0;
	case (0 + 1):
		return 1;
	case (2 + 0):
		return 3;
	case (2 + 1):
		return 2;
	}
	throw std::runtime_error("input for curveBit is not 0-3 integer");
}

static int MirrorLeftBottom(int L) {
	switch (L) {
	case 1:
		return 3;
	case 3:
		return 1;
	}
	return L;
}
static int MirrorRightBottom(int L) {
	switch (L) {
	case 2:
		return 0;
	case 0:
		return 2;
	}
	return L;
}
static int MirrorIfNeeded(int L, int previousXY) {
	if (previousXY == 0) return MirrorLeftBottom(L);
	if (previousXY == 3) return MirrorRightBottom(L);
	return L;
}

int HilbertCurve(ivec2 xy, int bitSize) {
	if (xy.x > 1 << bitSize || xy.y > 1 << bitSize) {
		throw std::runtime_error("input is out of bounds");
		return -1;
	}
	int output, xMasked, yMasked, xyMasked, mask;
	mask = 1;
	output = 0;
	int previousXY = 1;
	for (int i = 0; i < bitSize; i++){
		xMasked = (xy.x & mask) << 1;
		yMasked = (xy.y & mask);
		xyMasked = (xMasked + yMasked) >> i;
		int L = bitConvertXYtoL(xyMasked);
		L = MirrorIfNeeded(L, previousXY);
		previousXY = xyMasked;	//for the next cycle
		L = L << (2 * i);
		output += L;
		mask << 1;
	}
	return output;
}

struct ivec2 HilbertCurve(int l, int bitSize) {
	struct ivec2 output = { 0, 0 };
	int previousXY = 1;

	for (int i = 0; i < bitSize; i++) {
		int mask = (2 + 1) << (2 * i);
		int LMasked = (l & mask) >> (2 * i);
		LMasked = MirrorIfNeeded(LMasked, previousXY);
		int xyMasked = bitConvertXYtoL(LMasked);
		previousXY = xyMasked;
		int xMasked = (xyMasked >> 1) << i;
		int yMasked = (xyMasked & 1) << i;	
		output.x += xMasked;
		output.y += yMasked;
	}
	return output;
}

std::vector<color> HilbertCurve(std::vector<std::vector<color>> input2d, int bitSize) {
	std::vector<color> output;
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

void unitTestHilbert() {
	std::vector<std::vector<ivec2>> input;
	int bitSize = 2;
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