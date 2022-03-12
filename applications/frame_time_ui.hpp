#pragma once
#include "../impl/font_engine.hpp"
#include <chrono>
#include <memory>

namespace tinygui {

struct TGUIFrameDisplay {

	TGUIFrameDisplay(std::shared_ptr<TGUIFontEngine> pFontEngine);

	void setScreenPosition(unsigned xpos, unsigned ypos);

	void timeStart();
	void timeEnd();

	void histogramFrameTime();
	void histogramFPS();

	void drawFrameTime();
	void drawFPS();

private:

	unsigned displayPosX, displayPosY;
	std::shared_ptr<TGUIFontEngine> fontEngine;

	std::chrono::high_resolution_clock::time_point start, end;
	double elapsed_time;
};


}