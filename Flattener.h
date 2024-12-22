#pragma once
#include <iostream>
#include <vector>
struct ivec2 {
    int x;
    int y;
};
struct color {
    short int red;
    short int green;
    short int blue;
    short int alpha;
};

int HilbertCurve(ivec2 xy, int bitSize);
ivec2 HilbertCurve(int l, int bitSize);
std::vector<color> HilbertCurve(std::vector<std::vector<color>> input2d, int bitSize);
std::vector<int> HilbertCurve(std::vector<std::vector<int>> input2d, int bitSize);
std::vector<ivec2> HilbertCurve(std::vector<std::vector<ivec2>> input2d, int bitSize);
void unitTestHilbert();