#include "../headers/events.h"

static sfVector2i fieldSizes[3] = { {64, 48}, {80, 60}, {128, 72} };

static sfBool leftClick(sfEvent event);
static void clickHandle(float clickX, float clickY);

static void menuClick(float clickX, float clickY);
static void aboutClick(float clickX, float clickY);
static void settingsClick(float clickX, float clickY);
static void playClick(float clickX, float clickY);
static void updateInterfaceColors();

void eventHandle()
{
	sfEvent event;
	if (sfRenderWindow_pollEvent(window, &event))
	{
		if (event.type == sfEvtClosed)
		{
			sfRenderWindow_close(window);
			return;
		}
		else if (leftClick(event))
		{
			clickHandle(event.mouseButton.x, event.mouseButton.y);
		}
			
	}

}

void startFreqCounter()
{
	sfClock_restart(frequencyCounter.clock);
}

void resetFreqCounter()
{	
	float time = sfTime_asSeconds(sfClock_getElapsedTime(frequencyCounter.clock));
	if (time >= 1.00f)
	{
		char num[16] = "";
		snprintf(num, 16u, "%.2f", frequencyCounter.counter / time);
		sfText_setString(frequencyCounter.text, num);
		frequencyCounter.counter = 0;
		sfClock_restart(frequencyCounter.clock);
	}
}

static sfBool leftClick(sfEvent event)
{
	return (event.type == sfEvtMouseButtonPressed && 
			event.mouseButton.button == sfMouseLeft);
}

static void clickHandle(float clickX, float clickY)
{
	switch (menuStatus)
	{
	case PLAYING:
		playClick(clickX, clickY);
		break;

	case MENU:
		menuClick(clickX, clickY);
		break;

	case ABOUT:
		aboutClick(clickX, clickY);
		break;

	case SETTINGS:
		settingsClick(clickX, clickY);
		break;
		
	}
}

static void menuClick(float clickX, float clickY)
{
	if (sfFloatRect_contains(&mainMenu[B_PLAY].Bound, clickX, clickY))
	{
		menuStatus = PLAYING;
		//TODO: Change createButton to size/position because there is no 
		//necessity in full recreation
		createButton(&controlMenu[B_BACK], CONTROLMENU_BUTTON_SIZE, (sfVector2f) { 0, 548 - 1 }, "Back");
		sfRectangleShape_setFillColor(controlMenu[B_BACK].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
	}
	else if (sfFloatRect_contains(&mainMenu[B_SETTINGS].Bound, clickX, clickY))
	{
		menuStatus = SETTINGS;
		//TODO: Change createButton to size/position because there is no 
		//necessity in full recreation
		createButton(&controlMenu[B_BACK], (sfVector2f) { 192.f, 72.f }, (sfVector2f) { 768.f, 608.f }, "Back");
		sfRectangleShape_setFillColor(controlMenu[B_BACK].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
	}
	else if (sfFloatRect_contains(&mainMenu[B_ABOUT].Bound, clickX, clickY))
	{
		menuStatus = ABOUT;
		//TODO: Change createButton to size/position because there is no 
		//necessity in full recreation
		createButton(&controlMenu[B_BACK], (sfVector2f) { 192.f, 72.f }, (sfVector2f) { 768.f, 608.f }, "Back");
		sfRectangleShape_setFillColor(controlMenu[B_BACK].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
	}
	else if (sfFloatRect_contains(&mainMenu[B_EXIT].Bound, clickX, clickY))
	{
		sfRenderWindow_close(window);
		return;
	}
}

static void aboutClick(float clickX, float clickY)
{
	if (sfFloatRect_contains(&controlMenu[B_BACK].Bound, clickX, clickY))
	{
		menuStatus = MENU;
	}
}

static void settingsClick(float clickX, float clickY)
{
	if (sfFloatRect_contains(&settingsMenu[B_SIZE].Bound, clickX, clickY))
	{
		switch (FieldMode)
		{
		case SMALL:
			FieldMode = MEDIUM;
			sfText_setString(settingsMenu[B_SIZE].Text, "80x60");

			break;

		case MEDIUM:
			FieldMode = LARGE;
			sfText_setString(settingsMenu[B_SIZE].Text, "128x72");
			break;

		case LARGE:
			FieldMode = SMALL;
			sfText_setString(settingsMenu[B_SIZE].Text, "64x48");
			break;

		}
		recreatrFieldNet();
	}
	else if (sfFloatRect_contains(&settingsMenu[B_BORDER].Bound, clickX, clickY))
	{
		switch (BorderlessStatus)
		{
		case YES:
			BorderlessStatus = NO;
			sfText_setString(settingsMenu[B_BORDER].Text, "Off");
			break;

		case NO:
			BorderlessStatus = YES;
			sfText_setString(settingsMenu[B_BORDER].Text, "On");
			break;

		}
	}
	else if (sfFloatRect_contains(&settingsMenu[B_THEME].Bound, clickX, clickY))
	{
		switch (enumTheme)
		{
		case MATRIX:
			enumTheme = OLD;
			memcpy(CurTheme, OldTheme, sizeof(CurTheme));
			sfText_setString(settingsMenu[B_THEME].Text, "Old");
			break;

		case OLD:
			enumTheme = MATRIX;
			memcpy(CurTheme, MatrixTheme, sizeof(CurTheme));
			sfText_setString(settingsMenu[B_THEME].Text, "Matrix");
			break;

		}

		updateInterfaceColors();
	}
	else if (sfFloatRect_contains(&controlMenu[B_BACK].Bound, clickX, clickY))
	{
		menuStatus = MENU;
	}
}

void playClick(float clickX, float clickY)
{
	if (sfFloatRect_contains(&controlMenu[B_PLAY].Bound, clickX, clickY))
	{
		switch (PlayingStatus)
		{
		case GO:
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			sfText_setString(controlMenu[B_PLAY].Text, "Start");
			break;

		case STOP:
			PlayingStatus = GO;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
			sfText_setString(controlMenu[B_PLAY].Text, "Stop");
			break;

		}
	}
	else if (sfFloatRect_contains(&controlMenu[B_ONESTEP].Bound, clickX, clickY))
	{
		if (PlayingStatus == GO)
		{
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			sfText_setString(controlMenu[B_PLAY].Text, "Start");
		}
		nextGen();
	}
	else if (sfFloatRect_contains(&controlMenu[B_CLEAR].Bound, clickX, clickY))
	{
		if (PlayingStatus == GO)
		{
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			sfText_setString(controlMenu[B_PLAY].Text, "Start");
		}
		memset(cells, 0, 130*74);
	}
	else if (sfFloatRect_contains(&controlMenu[B_RANDOM].Bound, clickX, clickY))
	{
		srand(time(NULL));
		for (int j = 1; j < fieldSizes[FieldMode].y + 1; j++)
			for (int i = 1; i < fieldSizes[FieldMode].x + 1; i++)
			{
				cells[j * (fieldSizes[FieldMode].x + 2) + i] = rand() % 2;
			}
	}
	else if (sfFloatRect_contains(&controlMenu[B_BACK].Bound, clickX, clickY))
	{
		if (PlayingStatus == GO)
		{
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			sfText_setString(controlMenu[B_PLAY].Text, "Start");
		}
		menuStatus = MENU;
	}
	else
	{
		int cellCoordX = (int)(clickX - (1280 / 2 - fieldSizes[FieldMode].x * 7 / 2)) / 7;
		int cellCoordY = (int)(clickY - 180) / 7;
		if (cellCoordX >= 0 && cellCoordX < fieldSizes[FieldMode].x &&
			cellCoordY >= 0 && cellCoordY < fieldSizes[FieldMode].y)
		{
			cells[cellCoordX + 1 + (cellCoordY + 1) * (fieldSizes[FieldMode].x + 2)] = !cells[cellCoordX + 1 + (cellCoordY + 1) * (fieldSizes[FieldMode].x + 2)];
		}
		//TODO: Change vertex in aliveCells array 
	}
	for (struct sPatterns* TmpPattern = &startPattern; TmpPattern != NULL; TmpPattern = TmpPattern->next)
	{
		if (sfFloatRect_contains(&TmpPattern->Button.Bound, clickX, clickY))
		{
			memset(cells, 0, sizeof(cells));
			for (int j = 0; j < 64; j++)
				for (int k = 0; k < 48; k++)
					cells[j + 1 + (k + 1) * (fieldSizes[FieldMode].x + 2)] = TmpPattern->Pattern[j][k];

			break;
		}
	}

	//TODO: optimize some elements
}

static void updateInterfaceColors()
{
	for (int i = 0; i < 4; i++)
	{
		if (i < 3)
		{
			sfRectangleShape_setFillColor(settingsMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
		}
		sfRectangleShape_setFillColor(mainMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor]));

		sfRectangleShape_setFillColor(controlMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
	}
	sfRectangleShape_setFillColor(controlMenu[B_BACK].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
	for (struct sPatterns* tmpPattern = &startPattern; tmpPattern != NULL; tmpPattern = tmpPattern->next)
	{
		sfRectangleShape_setFillColor(tmpPattern->Button.Shape, ThemeTosfColor(CurTheme[ButtonColor]));
	}
	
	sfText_setFillColor(gameName, ThemeTosfColor(CurTheme[ButtonColor]));

	size_t aliveCellsCount = sfVertexArray_getVertexCount(field.aliveCells);
	for (size_t i = 0; i < aliveCellsCount; i++)
	{
		sfVertexArray_getVertex(field.aliveCells, i)->color = ThemeTosfColor(CurTheme[ButtonColor]);
	}

	size_t netSize = sfVertexArray_getVertexCount(field.fieldNet);
	for (size_t i = 0; i < netSize; i++)
	{
		sfVertexArray_getVertex(field.fieldNet, i)->color = ThemeTosfColor(CurTheme[ButtonColor]);
	}
}
