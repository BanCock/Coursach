#include "params.h"


void setStartState()
{
    PlayingStatus = STOP;
    FieldMode = LARGE;
    menuStatus = MENU;
    BorderlessStatus = YES;
    enumTheme = MATRIX;
    memcpy(CurTheme, MatrixTheme, sizeof(CurTheme));
    memset(field, 0, sizeof(field));
    sfRenderWindow_clear(window, ThemeTosfColor(CurTheme[WallColor]));
}
