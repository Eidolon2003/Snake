#include "snake.h"

int main() {
	jaw::EngineProperties ep;
	jaw::AppProperties ap;
	jaw::StartEngine(new Snake, ap, ep);
}