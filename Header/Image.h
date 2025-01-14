#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <complex>

typedef std::vector<std::vector<int>> IntMatrix;

struct Color {
    short int r, g, b;

    Color();
    Color(int red, int green, int blue);
    ~Color();

    static std::vector<int> GetVectorOfColor(std::vector<Color>, int colorIndex);
    static IntMatrix GetColorVectors(std::vector<Color> colors);
    static void ExtractColorVectorsTo(std::vector<Color> colors, std::vector<std::vector<int>> &colorsDivided);
    static Color ConvertComplexToColor(double magnitude, double phase);
};
class Image {
public:

    Image(int bitSize);
    Image(int width, int height);
    Image(int width, int height, IntMatrix red, IntMatrix green, IntMatrix blue);
    Image(int width, int height, IntMatrix white);
    Image(int width, int height, std::vector<std::vector<Color>> colors);
    ~Image();

    Color GetColor(int x, int y) const;
    void SetColor(Color color, int x, int y);
    std::vector<std::vector<Color>> GetColorArray();
    void ExtractColorArraysTo(std::vector<std::vector<std::vector<int>>> &colorsDivided);

    int GetBitSize();

    void Read(const std::string path);
    void ReadWithAlpha(const std::string path);
    void Export(std::string path) const;

private:
    static const int fileHeaderSize = 14;
    static const int informationHeaderSize = 40;
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
};

void ImageUnitTest();
Image TestImage();