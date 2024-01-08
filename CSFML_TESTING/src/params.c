#include "../headers/params.h"


void setStartState()
{
    PlayingStatus = STOP;
    FieldMode = LARGE;
    menuStatus = MENU;
    BorderlessStatus = YES;
    enumTheme = MATRIX;
    sfRenderWindow_clear(window, ThemeTosfColor(CurTheme[WallColor]));
}
