#include "Animation.h"

static double Sigmoid(double t, double exponent)
{
    if (t > 0.5) return (1 - Sigmoid(1 - t, exponent));
    return (pow(2, exponent-1) * pow(t,exponent));
}
static double ReverseSigmoid(double t, double exponent)
{
    return (Sigmoid(t, 1 / exponent));
}

AnimationSettings::AnimationSettings()
    :interpolationTrajectory(spiral), tempoCurveType(linear), fftType(twoDim), dynamicInterpolationType(off), sigmoidExponent(3.)
{
}
AnimationSettings::~AnimationSettings()
{
}


Animation::~Animation()
{
}
Animation::Animation(Image &FirstFrame, Image &LastFrame) 
    :m_firstFrame(FirstFrame), m_lastFrame(LastFrame)
{
    m_frameBitSize = FirstFrame.GetBitSize();
    m_frameSize = 1 << m_frameBitSize;
    Settings = AnimationSettings();
}

void Animation::FillFrames2d(int frameCount)
{
    std::cout << "Starting color extraction" << std::endl;
    std::vector<std::vector<std::vector<int>>> ColorsSeparated;
    m_firstFrame.ExtractColorArraysTo(ColorsSeparated);
    m_lastFrame.ExtractColorArraysTo(ColorsSeparated);
    std::cout << "Color extraction finished" << std::endl << std::endl;

    std::cout << "Starting comlex converting" << std::endl;
    std::vector<std::vector<ComplexArray>> ColorsComplex = IntToComplex(ColorsSeparated);
    std::cout << "Comlex converting finished" << std::endl << std::endl;

    std::cout << "Starting fft" << std::endl;
    std::vector<std::vector<ComplexArray>> fftsByColor = fft2d(ColorsComplex);
    std::cout << "Fft finished" << std::endl << std::endl;

    std::cout << "Starting creating frames" << std::endl;
    //Creating in-between frames
    for (double i = 1; i < frameCount - 1; i++) {
        for (int color = 0; color < 3; color++) {
            fftsByColor.push_back(Interpolate(fftsByColor[color], fftsByColor[color + 3], i / (frameCount - 1), m_frameSize));
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
    std::cout << "Reverse FFT finished" << std::endl << std::endl;

    std::cout << "Starting int converting" << std::endl;
    ColorsSeparated = ComplexToInt(ColorsComplex);
    std::cout << "Int converting finished" << std::endl << std::endl;

    std::cout << "Starting animation assembly" << std::endl;
    for (int i = 0; i < frameCount; i++) {
        Image frame = Image(m_frameSize, m_frameSize, ColorsSeparated[3 * i], ColorsSeparated[3 * i + 1], ColorsSeparated[3 * i + 2]);
        m_frames.push_back(frame);
    }
    std::cout << "Animation assembly finished" << std::endl << std::endl;
}
void Animation::FillFramesWithFlattening(int frameCount)
{
    std::cout << "Starting flattening" << std::endl;
    std::vector<Color> FirstFrameFlattened = HilbertCurve(m_firstFrame, m_frameBitSize);
    std::vector<Color> LastFrameFlattened = HilbertCurve(m_lastFrame, m_frameBitSize);
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
    for (double i = 1; i < frameCount - 1; i++) {
        for (int color = 0; color < 3; color++) {
            fftsByColor.push_back(Interpolate(fftsByColor[color], fftsByColor[color + 3], i / (frameCount - 1), m_frameSize * m_frameSize));
        }
    }
    //Putting LastFrame at the end of a list
    for (int color = 0; color < 3; color++) {
        fftsByColor.push_back(fftsByColor[3]);
        fftsByColor.erase(fftsByColor.begin() + 3);
    }
    std::cout << "Creating frames finished" << std::endl << std::endl;

    std::cout << "Starting reverse FFT" << std::endl;
    framesByColor = fftReverseToInt(fftsByColor, 3 * frameCount);   //Here maybe better not to do reverse-Fourier on first and last frames
    std::cout << "Reverse FFT finished" << std::endl << std::endl;

    std::cout << "Starting deflattening" << std::endl;
    std::vector<std::vector<std::vector<int>>> framesByColor2d = HilbertReverse(framesByColor, m_frameBitSize);    //back to 2d from hilbert
    std::cout << "Deflattening finished" << std::endl << std::endl;

    std::cout << "Starting animation assembly" << std::endl;
    for (int i = 0; i < frameCount; i++) {
        Image frame = Image(m_frameSize, m_frameSize, framesByColor2d[3 * i], framesByColor2d[3 * i + 1], framesByColor2d[3 * i + 2]);
        m_frames.push_back(frame);
    }
    std::cout << "Animation assembly finished" << std::endl << std::endl;
}

void Animation::FillFrames(int frameCount)
{
    switch (Settings.fftType) {
    case twoDim:
        FillFrames2d(frameCount);
        return;
    case oneDimWithFlattening:
        FillFramesWithFlattening(frameCount);
        return;
    }
    std::cout << "error: fftType not defined" << std::endl;
    return;
}

void Animation::Export()
{
    std::cout << "Starting export" << std::endl;
    for (int i = 0; i < m_frames.size(); i++) {
        std::string FrameName = "frame" + std::to_string(i) + ".bmp";
        m_frames[i].Export(FrameName);
    }
    std::cout << "Export finished" << std::endl << std::endl;
}


static std::complex<double> InterpolateLine(std::complex<double> x, std::complex<double> y, double t)
{
    return ((1 - t) * x + t * y);
}
static std::complex<double> InterpolateSpiral(std::complex<double> x, std::complex<double> y, double t)
{
    double absx = std::abs(x);
    double absy = std::abs(y);
    if (absx == 0) return (y * 0.5);
    if (absy == 0) return (x * 0.5);
    double magn = (1 - t) * absx + t * absy;
    double arg = (1 - t) * std::arg(x) + t * std::arg(y);
    std::complex<double> output = std::polar(magn, arg);
    return output;
}

std::complex<double> Animation::Interpolate(std::complex<double> x, std::complex<double> y, double t)
{
    switch (Settings.interpolationTrajectory) {
    case lineSegment:
        return (InterpolateLine(x, y, t));
    case spiral:
        return (InterpolateSpiral(x, y, t));
    }
    return (InterpolateSpiral(x, y, t));
}

ComplexArray Animation::Interpolate(ComplexArray X, ComplexArray Y, double t, int size) {
    ComplexArray output(size);
    double index = t;
    for (int i = 0; i < size; i++) {
        switch (Settings.dynamicInterpolationType) {
        case evenOdd:
            Settings.sigmoidExponent = 1 / Settings.sigmoidExponent;
            index = Sigmoid(t, Settings.sigmoidExponent);
            break;
        case hiLow:
            std::cout << "hiLow not implemented!" << std::endl << std::endl;
            break;
            index = Sigmoid(t, pow(Settings.sigmoidExponent, 2 * i / size));
        case off:
            break;
        }
        output[i] = Interpolate(X[i], Y[i], index);
    }
    return output;
}
ComplexMatrix Animation::Interpolate(ComplexMatrix X, ComplexMatrix Y, double t, int size) {
    std::vector<ComplexArray> output(size);
    double index = t;
    for (int i = 0; i < size; i++) {
        switch (Settings.dynamicInterpolationType) {
        case evenOdd:
            Settings.sigmoidExponent = 1 / Settings.sigmoidExponent;
            break;
        case hiLow:
            std::cout << "hiLow not implemented!" << std::endl << std::endl;
            break;
            index = Sigmoid(t, Settings.sigmoidExponent * 2 * i / size);
        case off:
            break;
        }
        output[i] = Interpolate(X[i], Y[i], index, size);
    }
    return output;
}