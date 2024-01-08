#include "game.h"

//hui

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
