#include "../headers/game.h"


void gameInit()
{
	windowCreate();
	setPalettes();
	setStartState();
	createFont();
	createMainMenu();
	createCountFreq();
	createControlMenu();
	createSettingsMenu();
	createAboutMenu();
	createField();
	createTitle();
	loadPatterns();
	setMusic();
}

void gameLoop()
{
	startFreqCounter();
	while (sfRenderWindow_isOpen(window))
	{
#ifndef _VOID_GAME
		eventHandle();
#endif // !_VOID_GAME
		interfaceDraw();
		fieldUpdate();
		resetFreqCounter();
	}
}

void deleteObjects()
{
	cleanInterface();
	cleanField();
	cleanPatterns();
	cleanMusic();
	cleanWindow();
}
