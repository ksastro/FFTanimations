#include "Image.h"
#include <fstream>
#include <iostream>

Color::Color() 
    : r(0), g(0), b(0)
{
}

Color::Color(int red, int green, int blue)
    : r(red), g(green), b(blue)
{
}

Color::~Color()
{
}



std::vector<int> Color::GetVectorOfColor(std::vector<Color> colors, int colorIndex)
{
    int size = (int)std::size(colors);
    //std::cout << size << std::endl;
    std::vector<int> output;
    for (int i = 0; i < size; i++) {
        switch (colorIndex) {
        case 0:
            //std::cout << 0 << std::endl;
            output.push_back(colors[i].r);
            break;
        case 1:
            //std::cout << 1 << std::endl;
            output.push_back(colors[i].g);
            break;
        case 2:
            //std::cout << 2 << std::endl;
            output.push_back(colors[i].b);
            break;
        }
    }
    return output;
}

std::vector<int> Color::GetRedVector(std::vector<Color> colors)
{
    return GetVectorOfColor(colors, 0);
}

std::vector<int> Color::GetGreenVector(std::vector<Color> colors)
{
    return GetVectorOfColor(colors, 1);
}

std::vector<int> Color::GetBlueVector(std::vector<Color> colors)
{
    return GetVectorOfColor(colors, 2);
}

std::vector<std::vector<int>> Color::GetColorVectors(std::vector<Color> colors) {
    int size = (int)std::size(colors);
    std::vector<std::vector<int>> output;
    for (int i = 0; i < size; i++) {
        output.push_back(GetVectorOfColor(colors, i));
    }
    return output;
}

void Color::ExtractColorVectorsTo(std::vector<Color> colors, std::vector<std::vector<int>>& colorsDivided) {
    for (int i = 0; i < 3; i++) {
        colorsDivided.push_back(GetVectorOfColor(colors, i));
    }
}


Image::Image(int width, int height)
    : m_width(width), m_height(height), m_colors(std::vector<Color>(width * height))
{
}

Image::Image(int width, int height, std::vector<std::vector<int>> red, std::vector<std::vector<int>> green, std::vector<std::vector<int>> blue)
    : m_width(width), m_height(height), m_colors(std::vector<Color>(width* height))
{
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            Color color = Color(red[x][y], green[x][y], blue[x][y]);
            m_colors[y * m_width + x] = color;
        }
    }
}

Image::~Image()
{
}

Color Image::GetColor(int x, int y) const
{
    return m_colors[y * m_width + x];
}

void Image::SetColor(Color color, int x, int y)
{
    m_colors[y * m_width + x] = color;
}

std::vector<std::vector<Color>> Image::GetColorArray()
{
    std::vector<std::vector<Color>> output(m_width, std::vector<Color>(m_height));
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            output[x][y] = (GetColor(x, y));
        }
    }
    return output;
}

void Image::ExtractColorArraysTo(std::vector<std::vector<std::vector<int>>> &colorsDivided)
{
    std::vector<std::vector<Color>> colors = GetColorArray();
    std::vector<std::vector<int>> red(m_width, std::vector<int>(m_height));
    std::vector<std::vector<int>> green(m_width, std::vector<int>(m_height));
    std::vector<std::vector<int>> blue(m_width, std::vector<int>(m_height));
    for (int x = 0; x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            Color color = GetColor(x, y);
            red[x][y] = color.r;
            green[x][y] = color.g;
            blue[x][y] = color.b;
        }
    }
    colorsDivided.push_back(red);
    colorsDivided.push_back(green);
    colorsDivided.push_back(blue);
}



void Image::ReadWithAlpha(const std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "File opening not successful";
        return;
    }

    unsigned char fileHeader[fileHeaderSize];
    file.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    unsigned char informationHeader[informationHeaderSize];
    file.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

    m_colors.resize(m_width * m_height);
    const int paddingAmount = (4 - (m_width * 3) % 4) % 4;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            unsigned char colorArray[3];
            file.read(reinterpret_cast<char*>(colorArray), 3);
            int r = static_cast<int>(colorArray[2]);
            int g = static_cast<int>(colorArray[1]);
            int b = static_cast<int>(colorArray[0]);

            Color color = Color(r, g, b);
            SetColor(color, x, y);
            file.ignore(1);         //Ignoring alpha
        }
        file.ignore(paddingAmount);
    }

    file.close();
}
void Image::Read(const std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open()) 
    {
        std::cout << "File opening not successful";
        return;
    }

    unsigned char fileHeader[fileHeaderSize];
    file.read(reinterpret_cast<char*>(fileHeader),fileHeaderSize);

    unsigned char informationHeader[informationHeaderSize];
    file.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

    m_colors.resize(m_width * m_height);
    const int paddingAmount = (4 - (m_width * 3) % 4) % 4;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            unsigned char colorArray[3];
            file.read(reinterpret_cast<char*>(colorArray), 3);
            int r = static_cast<int>(colorArray[2]);
            int g = static_cast<int>(colorArray[1]);
            int b = static_cast<int>(colorArray[0]);

            Color color = Color(r, g, b);
            SetColor(color, x, y);
        }
        file.ignore(paddingAmount);
    }

    file.close();
}

void Image::Export(std::string path) const
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        std::cout << "File opening not successful";
        return;
    }

    unsigned char bmpPadding[3] = {0, 0, 0};
    const int paddingAmount = (4 - (m_width * 3) % 4) % 4;

    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

    unsigned char fileHeader[fileHeaderSize];

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // Reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // Reserved 2 (not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    // Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    // Image width
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    // Image height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    // Planes
    informationHeader[12] = 0;
    informationHeader[13] = 0;
    // Bits per pixel (rgb)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    // Compression (no compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    // Image size (no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    // X pixels per meter (not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    // Y pixels per meter (not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    // Total colors (color palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    // Important colors (generally ignored)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    file.write(reinterpret_cast<char*>(fileHeader),fileHeaderSize);
    file.write(reinterpret_cast<char*>(informationHeader),informationHeaderSize);

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            Color color = GetColor(x, y);
            unsigned char r = color.r;
            unsigned char g = color.g;
            unsigned char b = color.b;
            unsigned char colorArray[] = { b, g, r };
            file.write(reinterpret_cast<char*>(colorArray), 3);
        }
        file.write(reinterpret_cast<char*>(bmpPadding), paddingAmount);
    }

    file.close();
    std::cout << "File created successfully\n";

}

void ImageUnitTest() {
    const int height = 512;
    const int width = 512;

    Image image(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image.SetColor(Color(x/2,y/2,255 - (x+y)/4),x,y);
        }
    }
    image.Export("image.bmp");
}
Image TestImage() {
    const int height = 2;
    const int width = 2;

    Image image(width, height);

    image.SetColor(Color(255,0,0), 0, 0);
    image.SetColor(Color(0,255,0), 0, 1);
    image.SetColor(Color(0,0,255), 1, 0);
    image.SetColor(Color(255,255,255), 1, 1);
    image.Export("image.bmp");
    return image;
}