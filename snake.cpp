#include "snake.h"

void Snake::Init() {
	//Draw border around the edges
	jaw::Point size = pWindow->getProperties().size;
	pGraphics->FillRect(jaw::Rect(0, 0, size.x, TILE_SIZE), BORDER_COLOR, 0);
	pGraphics->FillRect(jaw::Rect(0, 0, TILE_SIZE, size.y), BORDER_COLOR, 0);
	pGraphics->FillRect(jaw::Rect(size.x, size.y, 0, size.y - TILE_SIZE), BORDER_COLOR, 0);
	pGraphics->FillRect(jaw::Rect(size.x, size.y, size.x - TILE_SIZE, 0), BORDER_COLOR, 0);

	//Keybinds
	auto up = [this]() { inputQueue.push(UP); };
	auto down = [this]() { inputQueue.push(DOWN); };
	auto left = [this]() { inputQueue.push(LEFT); };
	auto right = [this]() { inputQueue.push(RIGHT); };

	pInput->BindKeyDown(jaw::W, up);
	pInput->BindKeyDown(jaw::UP, up);
	pInput->BindKeyDown(jaw::S, down);
	pInput->BindKeyDown(jaw::DOWN, down);
	pInput->BindKeyDown(jaw::A, left);
	pInput->BindKeyDown(jaw::LEFT, left);
	pInput->BindKeyDown(jaw::D, right);
	pInput->BindKeyDown(jaw::RIGHT, right);

	srand((unsigned)time(nullptr));
	Reset();
}

void Snake::Loop() {
	//Process a single input from the queue if available
	if (!inputQueue.empty()) {
		do {
			if (inputQueue.front() == UP && snakeDir != DOWN) break;
			if (inputQueue.front() == DOWN && snakeDir != UP) break;
			if (inputQueue.front() == LEFT && snakeDir != RIGHT) break;
			if (inputQueue.front() == RIGHT && snakeDir != LEFT) break;
			inputQueue.pop();
		} while (!inputQueue.empty());

		if (!inputQueue.empty()) {
			snakeDir = inputQueue.front();
			inputQueue.pop();
		}
	}

	//Move the snake according to its direction
	switch (snakeDir) {
	case UP:
		snakeParts.pop_back();
		snakeParts.push_front(jaw::Point(snakeParts.front().x, snakeParts.front().y - 1));
		break;

	case DOWN:
		snakeParts.pop_back();
		snakeParts.push_front(jaw::Point(snakeParts.front().x, snakeParts.front().y + 1));
		break;

	case LEFT:
		snakeParts.pop_back();
		snakeParts.push_front(jaw::Point(snakeParts.front().x - 1, snakeParts.front().y));
		break;

	case RIGHT:
		snakeParts.pop_back();
		snakeParts.push_front(jaw::Point(snakeParts.front().x + 1, snakeParts.front().y));
		break;

	case NONE:
	default:
		break;
	}

	//Check for collision with the apple
	if (snakeParts.front() == applePos) {
		snakeParts.push_back(snakeParts.back());

		do {
			applePos = RandPos();
		} while (std::find(snakeParts.cbegin(), snakeParts.cend(), applePos) != snakeParts.cend());
	}

	//Draw the apple, snake, and score
	DrawTile(APPLE_COLOR, applePos);
	std::for_each(snakeParts.cbegin(), snakeParts.cend(), std::bind_front(&Snake::DrawTile, this, SNAKE_COLOR));
	pGraphics->DrawString(L"Score: " + std::to_wstring(snakeParts.size()), jaw::Rect(GAP_SIZE, GAP_SIZE, INT16_MAX, INT16_MAX), 1);

	//Check for collision with self
	//Don't check for collision on the first four segments
	//They start overlapping, and it's impossible for them to collide
	auto itr = snakeParts.cbegin();
	for (int i = 0; i < 4; i++, itr++);

	bool hit = std::find(itr, snakeParts.cend(), snakeParts.front()) != snakeParts.cend();

	//Check for collision with the walls
	if (snakeParts.front().x >= NUM_TILES || snakeParts.front().x < 0 || snakeParts.front().y >= NUM_TILES || snakeParts.front().y < 0)
		hit = true;

	//If there was a collision, reset the game
	if (hit) Reset();
}

void Snake::Reset() {
	snakeDir = NONE;
	snakeParts.clear();
	auto snakePos = RandPos();
	for (int i = 0; i < 4; i++) snakeParts.push_back(snakePos);

	do {
		applePos = RandPos();
	} while (applePos == snakePos);
}

jaw::Point Snake::RandPos() const {
	return jaw::Point(rand() % NUM_TILES, rand() % NUM_TILES);
}

void Snake::DrawTile(uint32_t color, jaw::Point tile) const {
	uint16_t x = (tile.x + 1) * TILE_SIZE + GAP_SIZE;
	uint16_t y = (tile.y + 1) * TILE_SIZE + GAP_SIZE;
	jaw::Rect rect(x, y, x + TILE_SIZE - GAP_SIZE, y + TILE_SIZE - GAP_SIZE);
	pGraphics->FillRect(rect, color, 1);
}