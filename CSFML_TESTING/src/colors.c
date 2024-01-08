#include "../headers/colors.h"



void setPalettes()
{
    OldTheme[ButtonColor].HexColor = 0xa0a08bff;
    OldTheme[BackButtonColor].HexColor = 0x555568ff;
    OldTheme[NetColor].HexColor = 0x555568ff;
    OldTheme[WallColor].HexColor = 0x211e20ff;
    OldTheme[TextColor].HexColor = 0xe9efecff;

    MatrixTheme[ButtonColor].HexColor = 0x00c800ff;
    MatrixTheme[BackButtonColor].HexColor = 0xc80000ff;
    MatrixTheme[NetColor].HexColor = 0x006400ff;
    MatrixTheme[WallColor].HexColor = 0x000000ff;
    MatrixTheme[TextColor].HexColor = 0xffffffff;

    memcpy(CurTheme, MatrixTheme, sizeof(CurTheme));
}

sfColor ThemeTosfColor(union uPalete Theme)
{
    return (sfColor) { Theme.RGBAColors[R], Theme.RGBAColors[G], Theme.RGBAColors[B], Theme.RGBAColors[A] };
}