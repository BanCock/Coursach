#include "events.h"


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
	while (sfRenderWindow_pollEvent(window, &event))
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

static sfBool leftClick(sfEvent event)
{
	return (event.type == sfEvtMouseButtonPressed && 
			event.mouseButton.button == sfMouseLeft);
}

static void clickHandle(float clickX, float clickY)
{
	switch (menuStatus)
	{
	case MENU:
		menuClick(clickX, clickY);
		break;

	case ABOUT:
		aboutClick(clickX, clickY);
		break;

	case PLAYING:
		//TODO: Handle the field interaction
		playClick(clickX, clickY);
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
	}
	else if (sfFloatRect_contains(&mainMenu[B_SETTINGS].Bound, clickX, clickY))
	{
		menuStatus = SETTINGS;
		//TODO: Change createButton to size/position because there is no 
		//necessity in full recreation
		createButton(&controlMenu[B_BACK], (sfVector2f) { 192.f, 72.f }, (sfVector2f) { 768.f, 608.f }, "Back");
	}
	else if (sfFloatRect_contains(&mainMenu[B_ABOUT].Bound, clickX, clickY))
	{
		menuStatus = ABOUT;
		//TODO: Change createButton to size/position because there is no 
		//necessity in full recreation
		createButton(&controlMenu[B_BACK], (sfVector2f) { 192.f, 72.f }, (sfVector2f) { 768.f, 608.f }, "Back");
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
			strcpy(settingsMenu[B_SIZE].Text, "80x60");
			break;

		case MEDIUM:
			FieldMode = LARGE;
			strcpy(settingsMenu[B_SIZE].Text, "128x72");
			break;

		case LARGE:
			FieldMode = SMALL;
			strcpy(settingsMenu[B_SIZE].Text, "64x48");
			break;

		}
	}
	else if (sfFloatRect_contains(&settingsMenu[B_BORDER].Bound, clickX, clickY))
	{
		switch (BorderlessStatus)
		{
		case YES:
			BorderlessStatus = NO;
			strcpy(settingsMenu[B_BORDER].Text, "Off");
			break;

		case NO:
			BorderlessStatus = YES;
			strcpy(settingsMenu[B_BORDER].Text, "On");
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
			strcpy(settingsMenu[B_THEME].Text, "Old");
			break;

		case OLD:
			enumTheme = MATRIX;
			memcpy(CurTheme, MatrixTheme, sizeof(CurTheme));
			strcpy(settingsMenu[B_THEME].Text, "Matrix");
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
			strcpy(controlMenu[B_PLAY].Text, "Start");
			break;

		case STOP:
			PlayingStatus = GO;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
			strcpy(controlMenu[B_PLAY].Text, "Stop");
			break;

		}
	}
	else if (sfFloatRect_contains(&controlMenu[B_ONESTEP].Bound, clickX, clickY))
	{
		if (PlayingStatus == GO)
		{
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			strcpy(controlMenu[B_PLAY].Text, "Start");
		}
		//TODO: NewGeneration func 
	}
	else if (sfFloatRect_contains(&controlMenu[B_CLEAR].Bound, clickX, clickY))
	{
		if (PlayingStatus == GO)
		{
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			strcpy(controlMenu[B_PLAY].Text, "Start");
		}
		memset(field, 0, sizeof(field));
	}
	else if (sfFloatRect_contains(&controlMenu[B_RANDOM].Bound, clickX, clickY))
	{
		srand(time(NULL));
		for (int i = 0; i < fieldSizes[FieldMode].x; i++)
			for (int j = 0; j < fieldSizes[FieldMode].y; j++)
			{
				field[i][j] = rand() % 2;
			}
	}
	else if (sfFloatRect_contains(&controlMenu[B_BACK].Bound, clickX, clickY))
	{
		if (PlayingStatus == GO)
		{
			PlayingStatus = STOP;
			sfRectangleShape_setFillColor(controlMenu[B_PLAY].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
			strcpy(controlMenu[B_PLAY].Text, "Start");
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
			field[cellCoordX][cellCoordY] = !field[cellCoordX][cellCoordY];
		}
	}
	for (struct sPatterns* TmpPattern = &startPattern; TmpPattern != NULL; TmpPattern = TmpPattern->next)
	{
		if (sfFloatRect_contains(&TmpPattern->Button.Bound, clickX, clickY))
		{
			memset(field, 0, sizeof(field));
			for (int j = 0; j < 64; j++)
				for (int k = 0; k < 48; k++)
					field[j][k] = TmpPattern->Pattern[j][k];
			break;
		}
	}


	//TODO: optimize some elements
}

static void updateInterfaceColors()
{
	struct sPatterns* tmpPattern = &startPattern;

	for (int i = 0; i < 5; i++)
	{
		if (i < 3)
		{
			sfRectangleShape_setFillColor(settingsMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor])); 
		}
		if (i < 4) 
		{ 
			sfRectangleShape_setFillColor(mainMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor])); 
		}
		sfRectangleShape_setFillColor(controlMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
		sfRectangleShape_setFillColor(tmpPattern->Button.Shape, ThemeTosfColor(CurTheme[ButtonColor]));
		tmpPattern = tmpPattern->next;
	}
	
	sfText_setFillColor(gameName, ThemeTosfColor(CurTheme[ButtonColor]));

	//TODO: Update field colors
}
