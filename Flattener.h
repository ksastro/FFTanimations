#pragma once
#include <iostream>
#include <vector>
#include "Image.h"

typedef std::vector<std::vector<int>> IntMatrix;

struct ivec2 {
    int x;
    int y;
};


int HilbertCurve(ivec2 xy, int bitSize);
ivec2 HilbertCurve(int l, int bitSize);
std::vector<Color> HilbertCurve(std::vector<std::vector<Color>> input2d, int bitSize);
std::vector<int> HilbertCurve(IntMatrix input2d, int bitSize);
std::vector<ivec2> HilbertCurve(std::vector<std::vector<ivec2>> input2d, int bitSize);
std::vector<Color> HilbertCurve(Image image, int bitSize);

IntMatrix HilbertReverse(std::vector<int> curve, int bitSize);
std::vector<IntMatrix> HilbertReverse(std::vector<std::vector<int>> curves, int bitSize);

void unitTestHilbert1();
void unitTestHilbert2();
void unitTestHilbert3();