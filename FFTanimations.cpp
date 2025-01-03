#include <iostream>
#include <vector>
#include <complex>
#include "Image.h"
#include "Animation.h"


int main()
{    
    const int NUMBER_OF_FRAMES = 24;
    const int bitSize = 9;
    const int imageSize = 1 << bitSize;

    Image FirstFrame = Image(imageSize, imageSize);
    Image LastFrame = Image(imageSize, imageSize);
    
    FirstFrame.ReadWithAlpha("Masha.bmp");
    LastFrame.ReadWithAlpha("Stepan.bmp");

    Animation animation = Animation(FirstFrame, LastFrame);

    animation.FillFrames(NUMBER_OF_FRAMES);

    animation.Export();    
    return 0;
}
