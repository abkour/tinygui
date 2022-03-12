#include "frame_time_ui.hpp"

namespace tinygui {

TGUIFrameDisplay::TGUIFrameDisplay(std::shared_ptr<FontEngine> pFontEngine) {
	displayPosX = 0;
	displayPosY = 0;
	fontEngine = pFontEngine;
}

void TGUIFrameDisplay::setScreenPosition(unsigned xpos, unsigned ypos) {
	displayPosX = xpos;
	displayPosY = ypos;
}

void TGUIFrameDisplay::timeStart() {
	start = std::chrono::high_resolution_clock::now();
}

void TGUIFrameDisplay::timeEnd() {
	end = std::chrono::high_resolution_clock::now();
	elapsed_time = std::chrono::duration<double>(end - start).count();
}

void TGUIFrameDisplay::histogramFrameTime() {}
void TGUIFrameDisplay::histogramFPS() {}

void TGUIFrameDisplay::drawFrameTime() {
	std::string elapsed_time_s = std::to_string(elapsed_time * 1000).substr(0, 4);
	fontEngine->renderLine("Frametime: " + elapsed_time_s + "ms", displayPosX, displayPosY);
}

void TGUIFrameDisplay::drawFPS() {
	std::string fps_s = std::to_string(1.f / elapsed_time).substr(0, 5);
	fontEngine->renderLine("FPS: " + fps_s, displayPosX, displayPosY);
}

}