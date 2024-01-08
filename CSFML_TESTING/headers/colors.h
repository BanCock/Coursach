#ifndef COLORS_H
#define COLORS_H

#include <SFML/Graphics.h>
#include <string.h>

void setPalettes();
sfColor ThemeTosfColor(union uPalete Theme);

union uPalete
{
    unsigned int HexColor;
    unsigned char RGBAColors[4];
};

union uPalete OldTheme[5];
union uPalete MatrixTheme[5];
union uPalete CurTheme[5];

enum
{
    A, B, G, R
};
enum
{
    ButtonColor, BackButtonColor, NetColor, WallColor, TextColor
};

#endif // !COLORS_H

