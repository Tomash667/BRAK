#include "Pch.h"
#include "Core.h"
#include "Game.h"

int main()
{
	Game* game = new Game;
	if(!game->Run())
		return 1;
	return 0;
}
