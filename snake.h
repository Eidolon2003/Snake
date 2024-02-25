#include <JawEngine.h>
#include <queue>

class Snake : public jaw::AppInterface {
	static const uint32_t BORDER_COLOR = 0x1111EE;
	static const uint32_t SNAKE_COLOR = 0x11EE11;
	static const uint32_t APPLE_COLOR = 0xEE1111;

	std::list<jaw::Point> snakeParts;
	jaw::Point applePos;

	enum DIR { UP, DOWN, LEFT, RIGHT, NONE };
	std::queue<DIR> inputQueue;
	DIR snakeDir = NONE;

	void Reset();
	jaw::Point RandPos() const;
	void DrawTile(uint32_t color, jaw::Point tile) const;

public:
	static const uint8_t NUM_TILES = 32;
	static const uint8_t TILE_SIZE = 16;
	static const uint8_t GAP_SIZE = 2;

	void Init() override;
	void Loop() override;
};