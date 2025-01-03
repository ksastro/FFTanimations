#pragma once
#include <vector>
#include <string>

struct Color {
    short int r, g, b;

    Color();
    Color(int red, int green, int blue);
    ~Color();

    static std::vector<int> GetVectorOfColor(std::vector<Color>, int colorIndex);
    static std::vector<int> GetRedVector(std::vector<Color>);
    static std::vector<int> GetGreenVector(std::vector<Color>);
    static std::vector<int> GetBlueVector(std::vector<Color>);
    static std::vector<std::vector<int>> GetColorVectors(std::vector<Color> colors);
    static void ExtractColorVectorsTo(std::vector<Color> colors, std::vector<std::vector<int>> &colorsDivided);
};
class Image {
public:
    static const int fileHeaderSize = 14;
    static const int informationHeaderSize = 40;
    Image(int bitSize);
    Image(int width, int height);
    Image(int width, int height, std::vector<std::vector<int>> red, std::vector<std::vector<int>> green, std::vector<std::vector<int>> blue);
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

    int m_width;
    int m_height;
    std::vector<Color> m_colors;

    void m_SetFFTsFromColors();
    void m_SetColorsFromFFTs();
};

void ImageUnitTest();
Image TestImage();