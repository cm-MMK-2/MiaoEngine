#include "GameApp.hpp"

// The MAIN function, from here we start the application and run the game loop
int main()
{
	GameApp gameApp;
	if (Game::Create(1280, 720, &gameApp) == 0)
	{
		game->CreateGameWindow(u8"Game");
	}
}