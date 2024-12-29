#pragma once
#include <iostream>
#include <vector>
#include "Image.h"

struct ivec2 {
    int x;
    int y;
};


int HilbertCurve(ivec2 xy, int bitSize);
ivec2 HilbertCurve(int l, int bitSize);
std::vector<Color> HilbertCurve(std::vector<std::vector<Color>> input2d, int bitSize);
std::vector<int> HilbertCurve(std::vector<std::vector<int>> input2d, int bitSize);
std::vector<ivec2> HilbertCurve(std::vector<std::vector<ivec2>> input2d, int bitSize);
std::vector<Color> HilbertCurve(Image image, int bitSize);

std::vector<std::vector<int>> HilbertReverse(std::vector<int> curve, int bitSize);
std::vector<std::vector<std::vector<int>>> HilbertReverse(std::vector<std::vector<int>> curves, int bitSize);

void unitTestHilbert();