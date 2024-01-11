#include "../headers/params.h"


void setStartState()
{
#ifndef _VOID_GAME
    PlayingStatus = STOP;
 // !_VOID_GAME
#else
    PlayingStatus = GO;
#endif
    FieldMode = LARGE;
    menuStatus = MENU;
    BorderlessStatus = YES;
    enumTheme = MATRIX;
    sfRenderWindow_clear(window, ThemeTosfColor(CurTheme[WallColor]));
}
