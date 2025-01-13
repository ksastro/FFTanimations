#include <iostream>
#include <string>
#include "Image.h"
#include "Animation.h"



int main()
{    
    const int NUMBER_OF_FRAMES = 13;
    const int bitSize = 9;
    const int imageSize = 1 << bitSize;

    Image FirstFrame = Image(imageSize, imageSize);
    Image LastFrame = Image(imageSize, imageSize);

    

    FirstFrame.ReadWithAlpha("Water1.bmp");
    LastFrame.ReadWithAlpha("Water2.bmp");

    //FirstFrame.ReadWithAlpha("Masha.bmp");
    //LastFrame.ReadWithAlpha("Stepan.bmp");

    Animation animation(FirstFrame, LastFrame);

    animation.Settings.interpolationTrajectory = spiral;
    animation.Settings.dynamicInterpolationType = off;
    animation.Settings.fftType = twoDim;
    animation.Settings.tempoCurveType = sigmoid;
    animation.Settings.tempoCurveParameter = 3;

    animation.FillFrames(NUMBER_OF_FRAMES);

    animation.Export();
    return 0;
}