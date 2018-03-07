#include "GameApp.hpp"

// The MAIN function, from here we start the application and run the game loop
int main()
{
	GameApp gameApp;
	return Game::Create(800, 600, u8"Game", &gameApp);
}