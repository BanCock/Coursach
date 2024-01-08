#include "../headers/game.h"

//hui
//hui2
//hui3
//hui4
void gameInit()
{
	windowCreate();
	setPalettes();
	setStartState();
	createFont();
	createMainMenu();
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
	while (sfRenderWindow_isOpen(window))
	{
		eventHandle();
		interfaceDraw();
		fieldUpdate();
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
