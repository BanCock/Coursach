#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML/Graphics.h>
#include <string.h>

#include "params.h"
#include "colors.h"


#define MAINMENU_BUTTON_SIZE        (sfVector2f) { 320.f, 80.f }
#define CONTROLMENU_BUTTON_SIZE     (sfVector2f) { 181.f, 72.f }
#define SETTINGSMENU_BUTTON_SIZE    (sfVector2f) { 150.f, 50.f }


sfText* gameName;

struct Button mainMenu[4];
struct Button controlMenu[5];
struct Button settingsMenu[3];

void createButton(struct Button* button, sfVector2f size, sfVector2f position, const char* text);
void createMainMenu();
void createControlMenu();
void createSettingsMenu();
void createAboutMenu();
void createTitle();

void gameDraw();

struct Button
{
    sfRectangleShape* Shape;
    sfFloatRect Bound;
    sfText* Text;
};

#endif // !INTERFACE_H

