#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.h>
#include <stdio.h>
#include <string.h>

#include "params.h"
#include "colors.h"
#include "field.h"

#define MAINMENU_BUTTON_SIZE        (sfVector2f) { 320.f, 80.f }
#define CONTROLMENU_BUTTON_SIZE     (sfVector2f) { 181.f, 72.f }
#define SETTINGSMENU_BUTTON_SIZE    (sfVector2f) { 150.f, 50.f }

struct Button
{
    sfRectangleShape* Shape;
    sfFloatRect Bound;
    sfText* Text;
};

struct sPatterns
{
    int Pattern[64][48];
    struct Button Button;
    struct sPatterns* next;
};

struct countFreq
{
    int counter;
    sfClock* clock;
    sfText* text;
};

struct countFreq frequencyCounter;

sfText* gameName;

struct sPatterns startPattern;

struct Button mainMenu[4];
struct Button controlMenu[5];
struct Button settingsMenu[3];

void createFont();
void createButton(struct Button* button, sfVector2f size, sfVector2f position, const char* text);
void createMainMenu();
void createCountFreq();
void createControlMenu();
void createSettingsMenu();
void createAboutMenu();
void createTitle();

void loadPatterns();

void interfaceDraw();

void cleanInterface();
void cleanPatterns();

#endif // !INTERFACE_H

