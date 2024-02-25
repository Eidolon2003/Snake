#include "snake.h"

int main() {
	constexpr uint16_t SCREEN_SIZE = (Snake::NUM_TILES + 2) * Snake::TILE_SIZE + Snake::GAP_SIZE;

	jaw::EngineProperties ep;
	jaw::AppProperties ap;
	ap.layerCount = 2;
	ap.backgroundCount = 1;
	ap.framerate = 10;
	ap.size = jaw::Point(SCREEN_SIZE, SCREEN_SIZE);
	ap.title = "Snake";

	jaw::StartEngine(new Snake, ap, ep);
}