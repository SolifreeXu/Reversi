#include "Renderer.hpp"
#include "Game.hpp"

int main()
{
	using namespace reversi;
	Renderer renderer(25, 80, "Reversi");
	Game game(renderer);
	game.loop();
	return 0;
}
