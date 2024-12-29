// FFTanimations.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.


#include <iostream>
#include <vector>
#include <complex>
#include <iterator>
#include "Flattener.h"
#include "FFT.h"
#include "Image.h"

ComplexArray static Interpolate(ComplexArray X, ComplexArray Y, double t, int size) {
    ComplexArray output(size);
    for (int i = 0; i < size; i++) {
        output[i] = (1 - t) * X[i] + t * Y[i];
    }
    return output;
}

int main()
{     
    const int NUMBER_OF_FRAMES = 12;
    const int bitSize = 9;
    const int imageSize = 1 << bitSize;
    Image FirstFrame = Image(imageSize, imageSize);
    Image LastFrame = Image(imageSize, imageSize);
    std::vector<Image> animation;
    FirstFrame.Read("Water1.bmp");
    LastFrame.Read("Water2.bmp");

    std::cout << "Starting flattening" << std::endl;
    std::vector<Color> FirstFrameFlattened = HilbertCurve(FirstFrame, bitSize);
    std::vector<Color> LastFrameFlattened = HilbertCurve(LastFrame, bitSize);
    std::cout << "Flattening finished" << std::endl;

    std::vector<std::vector<int>> framesByColor;
    Color::ExtractColorVectorsTo(FirstFrameFlattened, framesByColor);
    Color::ExtractColorVectorsTo(LastFrameFlattened, framesByColor);
    
    //Fast Fourier Transform
    std::cout << "Starting FFT" << std::endl;
    std::vector<ComplexArray> fftsByColor = fftFromInt(framesByColor, 6);
    std::cout << "FFT finished" << std::endl;

    std::cout << "Starting creating frames" << std::endl;
    //Creating in-between frames
    for (double i = 1; i < NUMBER_OF_FRAMES - 1; i++) {
        for (int color = 0; color < 3; color++) {
            fftsByColor.push_back(Interpolate(fftsByColor[color], fftsByColor[color + 3], i / NUMBER_OF_FRAMES - 1, imageSize * imageSize));
        }
    }
    std::cout << "Creating frames finished" << std::endl;

    //Putting LastFrame at the end of a list
    /*for (int color = 0; color < 3; color++) {
        fftsByColor.push_back(fftsByColor[3]);
        fftsByColor.erase(fftsByColor.begin() + 3);
    }*/

    std::cout << "Starting reverse FFT" << std::endl;
    framesByColor = fftReverseToInt(fftsByColor, 3 * NUMBER_OF_FRAMES);   //Here maybe better not to do reverse-Fourier on first and last frames
    std::cout << "Reverse FFT finished" << std::endl;

    std::cout << "Starting deflattening" << std::endl;
    std::vector<std::vector<std::vector<int>>> framesByColor2d = HilbertReverse(framesByColor, bitSize);    //back to 2d from hilbert
    std::cout << "Deflattening finished" << std::endl;

    std::cout << "Starting export" << std::endl;
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        //Image frame = Image(1 << bitSize, 1 << bitSize);
        Image frame = Image(imageSize, imageSize, framesByColor2d[3 * i], framesByColor2d[3 * i + 1], framesByColor2d[3 * i + 2]);
        animation.push_back(frame);
        std::string FrameName = "frame" + std::to_string(i) + ".bmp";
        frame.Export(FrameName);
    }
    std::cout << "Export finished" << std::endl;

    return 0;
}
