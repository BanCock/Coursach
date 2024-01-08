#include "game.h"

//hui
//hui2
//hui3
void gameInit()
{
	windowCreate();
	setStartState();
	setPalettes();
	createMainMenu();
	createControlMenu();
	createSettingsMenu();
	createTitle();
	loadPatterns();
	setMusic();
}

void gameLoop()
{
	while (sfRenderWindow_isOpen(window))
	{
		eventHandle();
		gameDraw();
	}
}
