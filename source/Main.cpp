#include "Pch.h"
#include "Core.h"
#include "Game.h"

int main()
{
	Game* game = new Game;
	bool ok = true;
	if(!game->Run())
		ok = false;
	delete game;
	return ok ? 0 : 1;
}
