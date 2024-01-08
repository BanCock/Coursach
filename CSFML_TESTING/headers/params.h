#ifndef PARAMS_H
#define PARAMS_H

#include <string.h>

#include "colors.h"
#include "window.h"
#include "field.h"

void setStartState();

enum
{
    MENU,
    ABOUT,
    PLAYING,
    SETTINGS
} menuStatus;

enum
{
    SMALL,
    MEDIUM,
    LARGE
} FieldMode;

enum
{
    GO,
    STOP
} PlayingStatus;

enum
{
    YES,
    NO
} BorderlessStatus;

enum
{
    MATRIX,
    OLD
} enumTheme;

enum
{
    B_PLAY,
    B_SETTINGS,
    B_ABOUT,
    B_EXIT
};

enum
{
    B_START,
    B_ONESTEP,
    B_CLEAR,
    B_RANDOM,
    B_BACK
};

enum
{
    B_SIZE,
    B_BORDER,
    B_THEME
};
#endif // !PARAMS_H

