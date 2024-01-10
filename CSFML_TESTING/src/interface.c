#include "../headers/interface.h"

static sfRectangleShape* aboutBackground;
static sfText* aboutDescriptionHead;
static sfText* aboutDescriptionText;

static sfRectangleShape* settingstBackground;
static sfText* settingsHead;
static sfText* settingsText;

static sfFont* font;

static void drawMainMenu();
static void drawAbout();
static void drawSettings();
static void drawPlay();

void createFont()
{
    font = sfFont_createFromFile("BakbakOne-Regular.ttf");
}

void createButton(struct Button* button, sfVector2f size, sfVector2f position, const char* text)
{
	memset(button, 0, sizeof(struct Button));
	button->Shape = sfRectangleShape_create();
	sfRectangleShape_setSize(button->Shape, size);
	sfRectangleShape_setPosition(button->Shape, position);
	sfRectangleShape_setFillColor(button->Shape, ThemeTosfColor(CurTheme[ButtonColor]));

	button->Bound.width = size.x;
	button->Bound.height = size.y;

	button->Bound.left = position.x;
	button->Bound.top = position.y;

    button->Text = sfText_create();
    sfText_setString(button->Text, text);
    sfText_setFont(button->Text, font);
    sfText_setCharacterSize(button->Text, 40);

    sfVector2f TextPos =
    {
        position.x + (size.x - strlen(text) * 20) / 2, \
        position.y + (size.y - 40.0 * 3 / 2) / 2
    };

    sfText_setPosition(button->Text, TextPos);
    sfText_setFillColor(button->Text, ThemeTosfColor(CurTheme[TextColor]));
    
}

void createMainMenu()
{
    createButton(&mainMenu[B_PLAY], MAINMENU_BUTTON_SIZE, (sfVector2f) { 480.f, 260.f }, "Play");
    createButton(&mainMenu[B_SETTINGS], MAINMENU_BUTTON_SIZE, (sfVector2f) { 480.f, 360.f }, "Settings");
    createButton(&mainMenu[B_ABOUT], MAINMENU_BUTTON_SIZE, (sfVector2f) { 480.f, 460.f }, "About ");
    createButton(&mainMenu[B_EXIT], MAINMENU_BUTTON_SIZE, (sfVector2f) { 480.f, 560.f }, "Exit");
}

void createControlMenu()
{
    createButton(&controlMenu[B_START], CONTROLMENU_BUTTON_SIZE, (sfVector2f) { 0, 180 - 1 }, "Start");
    createButton(&controlMenu[B_ONESTEP], CONTROLMENU_BUTTON_SIZE, (sfVector2f) { 0, 272 - 1 }, "One step");
    createButton(&controlMenu[B_CLEAR], CONTROLMENU_BUTTON_SIZE, (sfVector2f) { 0, 364 - 1 }, "Clear");
    createButton(&controlMenu[B_RANDOM], CONTROLMENU_BUTTON_SIZE, (sfVector2f) { 0, 456 - 1 }, "Random ");
    createButton(&controlMenu[B_BACK], CONTROLMENU_BUTTON_SIZE, (sfVector2f) { 0, 548 - 1 }, "Back");
    sfRectangleShape_setFillColor(controlMenu[B_BACK].Shape, ThemeTosfColor(CurTheme[BackButtonColor]));
}

void createSettingsMenu()
{
    char text[7] = "128x72";
    if (FieldMode == MEDIUM) { strcpy(text, "80x60"); }
    else if (FieldMode == SMALL) { strcpy(text, "64x48"); }
    createButton(&settingsMenu[B_SIZE], SETTINGSMENU_BUTTON_SIZE, (sfVector2f) { 800.f, 301.f }, text);

    if (BorderlessStatus == YES) { strcpy(text, "On "); }
    else { strcpy(text, "Off"); }
    createButton(&settingsMenu[B_BORDER], SETTINGSMENU_BUTTON_SIZE, (sfVector2f) { 800.f, 361.f }, text);

    if (enumTheme == MATRIX) { strcpy(text, "Matrix"); }
    else if (enumTheme == OLD) { strcpy(text, "Old"); }
    createButton(&settingsMenu[B_THEME], SETTINGSMENU_BUTTON_SIZE, (sfVector2f) { 800.f, 421.f }, text);


    settingstBackground = sfRectangleShape_create();
    sfRectangleShape_setSize(settingstBackground, (sfVector2f) { 640, 480 });
    sfRectangleShape_setPosition(settingstBackground, (sfVector2f) { 320, 200 });
    sfRectangleShape_setFillColor(settingstBackground, (sfColor) { 100, 100, 100, 75 });

    settingsHead = sfText_create();
    settingsText = sfText_create();

    sfText_setFont(settingsHead, font);
    sfText_setFont(settingsText, font);

    sfText_setCharacterSize(settingsHead, 50);
    sfText_setCharacterSize(settingsText, 40);

    sfText_setString(settingsHead, "Settings");
    sfText_setString(settingsText, "Field Size\nBorderless Field\nTheme");

    sfText_setPosition(settingsHead, (sfVector2f) { 540, 200 });
    sfText_setPosition(settingsText, (sfVector2f) { 350, 300 });

    sfText_setFillColor(settingsHead, ThemeTosfColor(CurTheme[TextColor]));
    sfText_setFillColor(settingsText, ThemeTosfColor(CurTheme[TextColor]));
}

void createAboutMenu()
{   
    aboutBackground = sfRectangleShape_create();
    sfRectangleShape_setSize(aboutBackground, (sfVector2f) { 640, 480 });
    sfRectangleShape_setPosition(aboutBackground, (sfVector2f) { 320, 200 });
    sfRectangleShape_setFillColor(aboutBackground, (sfColor) { 100, 100, 100, 75 });

    aboutDescriptionHead = sfText_create();
    aboutDescriptionText = sfText_create();

    sfText_setFont(aboutDescriptionHead, font);
    sfText_setFont(aboutDescriptionText, font);

    sfText_setCharacterSize(aboutDescriptionHead, 40);
    sfText_setCharacterSize(aboutDescriptionText, 30);

    sfText_setString(aboutDescriptionHead, "Created by:\n\n\nFrom:");
    sfText_setString(aboutDescriptionText, "Ivanov Danil Sergeevich\n"
                                           "Shatalov Maksim Romanovich\n"
                                           "Sound by Rodion\n\n"
                                           
                                           "Saint Petersburg Polytechnic University\n"
                                           "Institute for Cyber Security\n"
                                           "and Information Protecton\n"
                                           "Group 4851001/20002\n\n"
                                           
                                           "(c) 2023 All Rights reserved");
    sfText_setPosition(aboutDescriptionHead, (sfVector2f) { 325.f, 200.f });
    sfText_setPosition(aboutDescriptionText, (sfVector2f) { 325.f, 250.f });
    
    sfText_setFillColor(aboutDescriptionHead, ThemeTosfColor(CurTheme[TextColor]));
    sfText_setFillColor(aboutDescriptionText, ThemeTosfColor(CurTheme[TextColor]));
}

void createTitle()
{
    gameName = sfText_create();
    sfText_setString(gameName, "Life the Game");

    sfFont* titleFont = sfFont_createFromFile("protosans.ttf");
    sfText_setFont(gameName, titleFont);
    sfText_setCharacterSize(gameName, 120);

    sfVector2f TextPos = { 120.f, 40.f }; // (1280 - strlen("Life the Game") * 80) / 2, 40

    sfText_setPosition(gameName, TextPos);

    sfText_setFillColor(gameName, ThemeTosfColor(CurTheme[ButtonColor]));
    //sfFont_destroy(titleFont);
}

void loadPatterns()
{
    FILE* filePattern = fopen("MyPatternsList.txt", "r");
    char patternName[10] = "";
    sfVector2f position =
    {
        1280 - 181, \
        180 - 1
    };


    memset(&startPattern, 0, sizeof(startPattern));
    fscanf(filePattern, "%s ", patternName);
    createButton(&startPattern.Button, CONTROLMENU_BUTTON_SIZE, position, patternName);

    for (int j = 0; j < 64; j++)
        for (int k = 0; k < 48; k++)
            startPattern.Pattern[j][k] = fgetc(filePattern) - 48;

    struct sPatterns* tmpPattern = &startPattern;
    for (int i = 1; i < 5; i++)
    {
        tmpPattern->next = calloc(1, sizeof(struct sPatterns));
        position.y += 92;

        fscanf(filePattern, " %s ", patternName);
        createButton(&tmpPattern->next->Button, CONTROLMENU_BUTTON_SIZE, position, patternName);

        for (int j = 0; j < 64; j++)
            for (int k = 0; k < 48; k++)
                tmpPattern->next->Pattern[j][k] = fgetc(filePattern) - 48;

        tmpPattern = tmpPattern->next;
    }
}

void interfaceDraw()
{
    sfRenderWindow_clear(window, ThemeTosfColor(CurTheme[WallColor]));

    sfRenderWindow_drawText(window, gameName, NULL);

    switch (menuStatus)
    {
    case MENU:
        drawMainMenu();
        break;

    case ABOUT:
        drawAbout();
        break;

    case PLAYING:
        drawPlay();
        break;

    case SETTINGS:
        drawSettings();
        break;

    }
    sfRenderWindow_display(window);
}

static void drawMainMenu()
{
    for (int i = 0; i < 4; i++)
    {
        sfRenderWindow_drawRectangleShape(window, mainMenu[i].Shape, NULL);
        sfRenderWindow_drawText(window, mainMenu[i].Text, NULL);
    }
}

static void drawAbout()
{
    sfRenderWindow_drawRectangleShape(window, aboutBackground, NULL);

    sfRenderWindow_drawText(window, aboutDescriptionHead, NULL);
    sfRenderWindow_drawText(window, aboutDescriptionText, NULL);

    sfRenderWindow_drawRectangleShape(window, controlMenu[B_BACK].Shape, NULL);
    sfRenderWindow_drawText(window, controlMenu[B_BACK].Text, NULL);
}

static void drawSettings()
{
    sfRenderWindow_drawRectangleShape(window, settingstBackground, NULL);

    sfRenderWindow_drawText(window, settingsHead, NULL);
    sfRenderWindow_drawText(window, settingsText, NULL);

    for (int i = 0; i < 3; i++)
    {
        sfRenderWindow_drawRectangleShape(window, settingsMenu[i].Shape, NULL);
        sfRenderWindow_drawText(window, settingsMenu[i].Text, NULL);
    }
    sfRenderWindow_drawRectangleShape(window, controlMenu[B_BACK].Shape, NULL);
    sfRenderWindow_drawText(window, controlMenu[B_BACK].Text, NULL);
}

static void drawPlay()
{
    fillAliveCells();
    sfRenderWindow_drawVertexArray(window, field.fieldNet, NULL);
    sfRenderWindow_drawVertexArray(window, field.aliveCells, NULL);

    for (int i = 0; i < 5; i++)
    {
        sfRenderWindow_drawRectangleShape(window, controlMenu[i].Shape, NULL);
        sfRenderWindow_drawText(window, controlMenu[i].Text, NULL);
    }
    
    for (struct sPatterns* tmpPattern = &startPattern; tmpPattern != NULL; tmpPattern = tmpPattern->next)
    {
        sfRenderWindow_drawRectangleShape(window, tmpPattern->Button.Shape, NULL);
        sfRenderWindow_drawText(window, tmpPattern->Button.Text, NULL);
    }

}

void cleanInterface()
{
    struct sPatterns* tmpPattern = &startPattern;

    for (int i = 0; i < 5; i++)
    {
        if (i < 3)
        {
            sfRectangleShape_destroy(settingsMenu[i].Shape);
            sfText_destroy(settingsMenu[i].Text);
        }
        if (i < 4)
        {
            sfRectangleShape_destroy(mainMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
            sfText_destroy(mainMenu[i].Text);
        }
        sfRectangleShape_destroy(controlMenu[i].Shape, ThemeTosfColor(CurTheme[ButtonColor]));
        sfText_destroy(controlMenu[i].Text);

        sfRectangleShape_destroy(tmpPattern->Button.Shape, ThemeTosfColor(CurTheme[ButtonColor]));
        sfText_destroy(tmpPattern->Button.Text);
        tmpPattern = tmpPattern->next;
    }

    sfRectangleShape_destroy(aboutBackground);
    sfRectangleShape_destroy(settingstBackground);

    sfText_destroy(settingsHead);
    sfText_destroy(settingsText);

    sfText_destroy(aboutDescriptionHead);
    sfText_destroy(aboutDescriptionText);

    sfFont_destroy(font);
}

void cleanPatterns()
{
    struct sPatterns* Pattern = startPattern.next;
    for (; Pattern != NULL;)
    {
        struct sPatterns* tmp = Pattern->next;
        free(Pattern);
        Pattern = tmp;
    }
}
