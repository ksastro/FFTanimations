#pragma once
#include <iostream>
#include <vector>
#include <complex>
#include "FFT.h"
#include "Image.h"
#include "Flattener.h"


enum InterpolationTrajectory { lineSegment, spiral };
enum TempoCurveType { linear, sigmoid, gammoid };
enum FftType { twoDim , oneDimWithFlattening};
enum DynamicInterpolationType {off, evenOdd, hiLow};

struct AnimationSettings
{
	InterpolationTrajectory interpolationTrajectory;
	TempoCurveType tempoCurveType;
	FftType fftType;
	DynamicInterpolationType dynamicInterpolationType;
	double tempoCurveParameter;

	AnimationSettings();
	~AnimationSettings();

};


class Animation {
public:
	AnimationSettings Settings;
	
	~Animation();
	Animation(Image &FirstFrame, Image &LastFrame);

	void FillFrames(int frameCount);
	void Export();


private:

	int m_frameBitSize;
	int m_frameSize;
	Image m_firstFrame;
	Image m_lastFrame;
	std::vector<Image> m_frames = {};

	std::complex<double> Interpolate(std::complex<double> x, std::complex<double> y, double t);
	ComplexArray Interpolate(ComplexArray X, ComplexArray Y, double t, int size);
	ComplexMatrix Interpolate(ComplexMatrix X, ComplexMatrix Y, double t, int size);

	void FillFramesWithFlattening(int frameCount);
	void FillFrames2d(int frameCount);
	double TempoCurve(double t, double parameter);
};

void GammoidUnitTest();