#include <iostream>
#include <vector>
#include <complex>
#include "Flattener.h"
#include "FFT.h"
#include "Image.h"


static std::complex<double> Interpolate(std::complex<double> x, std::complex<double> y, double t)
{
    double magn = (1 - t) * std::abs(x) + t * std::abs(y);
    double arg = (1 - t) * std::arg(x) + t * std::arg(y);
    std::complex<double> output = std::polar(magn,arg);
    return output;
}
static ComplexArray Interpolate(ComplexArray X, ComplexArray Y, double t, int size) {
    ComplexArray output(size);
    for (int i = 0; i < size; i++) {
        output[i] = Interpolate(X[i], Y[i], t);
    }
    return output;
}
static ComplexMatrix Interpolate(ComplexMatrix X, ComplexMatrix Y, double t, int size) {
    std::vector<ComplexArray> output(size);
    for (int i = 0; i < size; i++) {
        output[i] = Interpolate(X[i], Y[i], t, size);
    }
    return output;
}

int main()
{
    fft2dUnitTest();
    return 0;
    
    const int NUMBER_OF_FRAMES = 12;
    const int bitSize = 9;
    const int imageSize = 1 << bitSize;
    std::vector<Image> animation;


    Image FirstFrame = Image(imageSize, imageSize);
    Image LastFrame = Image(imageSize, imageSize);
    
    FirstFrame.ReadWithAlpha("Water1.bmp");
    LastFrame.ReadWithAlpha("Water2.bmp");

    std::cout << "Starting color extraction" << std::endl;
    std::vector<std::vector<std::vector<int>>> ColorsSeparated;
    FirstFrame.ExtractColorArraysTo(ColorsSeparated);
    LastFrame.ExtractColorArraysTo(ColorsSeparated);
    std::cout << "Color extraction finished" << std::endl << std::endl;

    std::cout << "Starting comlex converting" << std::endl;
    std::vector<std::vector<ComplexArray>> ColorsComplex = IntToComplex(ColorsSeparated);
    std::cout << "Comlex converting finished" << std::endl;

    std::cout << "Starting fft" << std::endl;
    std::vector<std::vector<ComplexArray>> fftsByColor = fft2d(ColorsComplex);
    std::cout << "Fft finished" << std::endl;

    std::cout << "Starting creating frames" << std::endl;
    //Creating in-between frames
    for (double i = 1; i < NUMBER_OF_FRAMES - 1; i++) {
        for (int color = 0; color < 3; color++) {
            fftsByColor.push_back(Interpolate(fftsByColor[color], fftsByColor[color + 3], i / (NUMBER_OF_FRAMES - 1), imageSize));
        }
    }

    //Putting LastFrame at the end of a list
    for (int color = 0; color < 3; color++) {
        fftsByColor.push_back(fftsByColor[3]);
        fftsByColor.erase(fftsByColor.begin() + 3);
    }
    std::cout << "Creating frames finished" << std::endl << std::endl;

    std::cout << "Starting reverse FFT" << std::endl;
    ColorsComplex = fftReverse2d(fftsByColor);
    std::cout << "Reverse FFT finished" << std::endl;

    std::cout << "Starting int converting" << std::endl;
    ColorsSeparated = ComplexToInt(ColorsComplex);
    std::cout << "Int converting finished" << std::endl;

    std::cout << "Starting export" << std::endl;
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        //Image frame = Image(1 << bitSize, 1 << bitSize);
        Image frame = Image(imageSize, imageSize, ColorsSeparated[3 * i], ColorsSeparated[3 * i + 1], ColorsSeparated[3 * i + 2]);
        animation.push_back(frame);
        std::string FrameName = "frame" + std::to_string(i) + ".bmp";
        frame.Export(FrameName);
    }
    std::cout << "Export finished" << std::endl << std::endl;
    












    /*
    std::cout << "Starting flattening" << std::endl;
    std::vector<Color> FirstFrameFlattened = HilbertCurve(FirstFrame, bitSize);
    std::vector<Color> LastFrameFlattened = HilbertCurve(LastFrame, bitSize);
    std::cout << "Flattening finished" << std::endl << std::endl;

    std::cout << "Starting color extraction" << std::endl;
    std::vector<std::vector<int>> framesByColor;
    Color::ExtractColorVectorsTo(FirstFrameFlattened, framesByColor);
    Color::ExtractColorVectorsTo(LastFrameFlattened, framesByColor);
    std::cout << "Color extraction finished" << std::endl << std::endl;

    std::cout << "Starting FFT" << std::endl;
    std::vector<ComplexArray> fftsByColor = fftFromInt(framesByColor, 6);
    std::cout << "FFT finished" << std::endl << std::endl;

    std::cout << "Starting creating frames" << std::endl;
    //Creating in-between frames
    for (double i = 1; i < NUMBER_OF_FRAMES - 1; i++) {
        for (int color = 0; color < 3; color++) {
            fftsByColor.push_back(Interpolate(fftsByColor[color], fftsByColor[color + 3], i / (NUMBER_OF_FRAMES - 1), imageSize * imageSize));
        }
    }
    std::cout << "Creating frames finished" << std::endl << std::endl;

    //Putting LastFrame at the end of a list
    for (int color = 0; color < 3; color++) {
        fftsByColor.push_back(fftsByColor[3]);
        fftsByColor.erase(fftsByColor.begin() + 3);
    }

    
    std::cout << "Starting reverse FFT" << std::endl;
    framesByColor = fftReverseToInt(fftsByColor, 3 * NUMBER_OF_FRAMES);   //Here maybe better not to do reverse-Fourier on first and last frames
    std::cout << "Reverse FFT finished" << std::endl << std::endl;

    std::cout << "Starting deflattening" << std::endl;
    std::vector<std::vector<std::vector<int>>> framesByColor2d = HilbertReverse(framesByColor, bitSize);    //back to 2d from hilbert
    std::cout << "Deflattening finished" << std::endl << std::endl;

    std::cout << "Starting export" << std::endl;
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        //Image frame = Image(1 << bitSize, 1 << bitSize);
        Image frame = Image(imageSize, imageSize, framesByColor2d[3 * i], framesByColor2d[3 * i + 1], framesByColor2d[3 * i + 2]);
        animation.push_back(frame);
        std::string FrameName = "frame" + std::to_string(i) + ".bmp";
        frame.Export(FrameName);
    }
    std::cout << "Export finished" << std::endl << std::endl;

    return 0;
    */
}
