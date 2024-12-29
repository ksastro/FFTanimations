#pragma once
#include "Image.h"
#include <vector>

class Animation {
public:
	Animation();
	~Animation();
	Animation(Image FirstFrame, Image LastFrame, int FramesCount);

	void FillFrames();
	void Export();

private:
	int m_framesCount;
	Image m_firstFrame;
	Image m_lastFrame;
	std::vector<Image> m_frames;
};