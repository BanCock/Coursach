#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <string.h>

//Размер окна
sfVector2i WindowSize = { 1280, 720 };
//Структура кнопки
typedef struct
{
    sfRectangleShape* Shape;
    sfFloatRect Bound;
    sfText* Text;
} struct Button;
//В каком меню мы сейчас находимся
enum
{
    MENU,
    ABOUT,
    PLAYING,
    SETTINGS
} menuStatus;
//Размеры поля
enum
{
    SMALL,
    MEDIUM,
    LARGE
} FieldMode;
//Состояние игры - пауза или генерация
enum
{
    GO,
    STOP
} PlayingStatus;
enum
{
    YES,
    NO
}BorderlessStatus;
enum
{
    MATRIX,
    OLD
} enumTheme;

typedef struct sPatterns
{
    int Pattern[64][48];
    struct Button Button;
    struct sPatterns* next;
}tPatterns;


union uPalete
{
    unsigned int HexColor;
    unsigned char RGBAColors[4];
}CurTheme[5];
enum
{
    A, B, G, R
};
enum
{
    ButtonColor, BackButtonColor, NetColor, WallColor, TextColor
};

sfColor ThemeTosfColor(union uPalete Theme)
{
    return (sfColor) { Theme.RGBAColors[R], Theme.RGBAColors[G], Theme.RGBAColors[B], Theme.RGBAColors[A] };
}

//Размеры поля (в количестве клеток)
sfVector2i FieldSizes[3] = { {64, 48}, {80, 60}, {128, 72} };

//Массив, составляющий игровое поле
int Field[128][72];

//Проверка на то, что координаты соответствуют полю
sfBool IsInField(int x, int y)
{
    return (x >= 0) && (y >= 0) && (x < FieldSizes[FieldMode].x) && (y < FieldSizes[FieldMode].y);
}
//Подсчёт количества живых соседей у клетки
int Neighbors(int x, int y)
{
    int counter = 0;
    if (BorderlessStatus == NO)
    {
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
            {
                if (IsInField(x + i, y + j) && (i || j) && Field[x + i][y + j] == 1) counter++;
            }
        return counter;
    }
    else
    {
        int TX = x, TY = y;
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
            {
                TX = x + i;
                TY = y + j;
                if (TX >= FieldSizes[FieldMode].x) TX = 0;
                else if (TX < 0) TX = FieldSizes[FieldMode].x - 1;
                if (TY >= FieldSizes[FieldMode].y) TY = 0;
                else if (TY < 0) TY = FieldSizes[FieldMode].y - 1;

                if ((i || j) && Field[TX][TY] == 1) counter++;
            }
        return counter;
    }
}
//Генерация нового поколения
void NewGeneration(void)
{
    int NextGen[128][72];
    int count = 0;
    memcpy(NextGen, Field, sizeof(Field));
    for (int i = 0; i < FieldSizes[FieldMode].x; i++)
        for (int j = 0; j < FieldSizes[FieldMode].y; j++)
        {
            count = Neighbors(i, j);
            if (Field[i][j])
            {
                if (count == 2 || count == 3) NextGen[i][j] = 1;
                else NextGen[i][j] = 0;
            }
            else
            {
                if (count == 3) NextGen[i][j] = 1;
            }
        }
    memcpy(Field, NextGen, sizeof(Field));
}



//Процедура для создания кнопки с текстом и заданным цветом
void CreateButton(struct Button* Button, sfVector2f Size, sfVector2f Position, const char* Text, sfColor BColor)
{
    memset(Button, 0, sizeof(struct Button));
    //Создаём форму кнопки - зелёный прямоугольник
    Button->Shape = sfRectangleShape_create();
    sfRectangleShape_setSize(Button->Shape, Size);
    sfRectangleShape_setPosition(Button->Shape, Position);
    sfRectangleShape_setFillColor(Button->Shape, BColor);
    Button->Bound.width = Size.x;
    Button->Bound.height = Size.y;
    Button->Bound.left = Position.x;
    Button->Bound.top = Position.y;
    //Создаём белый текст поверх кнопки
    sfFont* font = sfFont_createFromFile("BakbakOne-Regular.ttf");
    Button->Text = sfText_create();
    sfText_setString(Button->Text, Text);
    sfText_setFont(Button->Text, font);
    sfText_setCharacterSize(Button->Text, 40);
    sfVector2f TextPos =
    {
        Position.x + (Size.x - strlen(Text) * 20) / 2, \
        Position.y + (Size.y - 40.0 * 3 / 2) / 2
    };
    sfText_setPosition(Button->Text, TextPos);
    sfText_setFillColor(Button->Text, ThemeTosfColor(CurTheme[TextColor]));
}

//Отрисовка названия игры вверху экрана
void DrawGameName(sfText* GameName)
{
    sfText_setString(GameName, "Life the Game");
    sfFont* font = sfFont_createFromFile("protosans.ttf");
    sfText_setFont(GameName, font);
    sfText_setCharacterSize(GameName, 120);
    sfVector2f TextPos =
    {
        (1280 - strlen("Life the Game") * 80) / 2, \
        40
    };
    sfText_setPosition(GameName, TextPos);
    sfText_setFillColor(GameName, ThemeTosfColor(CurTheme[ButtonColor]));

}

//Процедура для создания главного меню
void CreateMainMenu(struct Button* PlayButton, struct Button* SettingsButton, struct Button* Aboustruct Button, struct Button* Existruct Button)
{
    CreateButton(PlayButton, (sfVector2f) { 320.f, 80.f }, (sfVector2f) { 480.f, 260.f }, "Play", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(SettingsButton, (sfVector2f) { 320.f, 80.f }, (sfVector2f) { 480.f, 360.f }, "Settings", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(Aboustruct Button, (sfVector2f) { 320.f, 80.f }, (sfVector2f) { 480.f, 460.f }, "About ", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(Existruct Button, (sfVector2f) { 320.f, 80.f }, (sfVector2f) { 480.f, 560.f }, "Exit", ThemeTosfColor(CurTheme[ButtonColor]));
}

//Процедура для отрисовки главного меню на экране
void DrawMainMenu(sfRenderWindow* Window, struct Button* PlayButton, struct Button* SettingsButton, struct Button* Aboustruct Button, struct Button* Existruct Button)
{
    sfRenderWindow_drawRectangleShape(Window, PlayButton->Shape, NULL);
    sfRenderWindow_drawText(Window, PlayButton->Text, NULL);
    sfRenderWindow_drawRectangleShape(Window, SettingsButton->Shape, NULL);
    sfRenderWindow_drawText(Window, SettingsButton->Text, NULL);
    sfRenderWindow_drawRectangleShape(Window, Aboustruct Button->Shape, NULL);
    sfRenderWindow_drawText(Window, Aboustruct Button->Text, NULL);
    sfRenderWindow_drawRectangleShape(Window, Existruct Button->Shape, NULL);
    sfRenderWindow_drawText(Window, Existruct Button->Text, NULL);
}

//Процедура отрисовки меню информации об игре
void CreateAboutMenu(sfRenderWindow* Window)
{
    sfRectangleShape* Back = sfRectangleShape_create();
    sfRectangleShape_setSize(Back, (sfVector2f) { 640, 480 });
    sfRectangleShape_setPosition(Back, (sfVector2f) { 320, 200 });
    sfRectangleShape_setFillColor(Back, (sfColor) { 100, 100, 100, 75 });
    sfRenderWindow_drawRectangleShape(Window, Back, NULL);


    sfFont* font = sfFont_createFromFile("BakbakOne-Regular.ttf");
    sfText* Description = sfText_create();
    sfText_setFont(Description, font);
    sfText_setCharacterSize(Description, 40);
    sfText_setString(Description, "Created by:\n\n\nFrom:");
    sfText_setPosition(Description, (sfVector2f) { 325, 200 });
    sfText_setFillColor(Description, ThemeTosfColor(CurTheme[TextColor]));
    sfRenderWindow_drawText(Window, Description, NULL);
    sfText_setCharacterSize(Description, 30);
    sfText_setString(Description, "Ivanov Danil Sergeevich\nShatalov Maksim Romanovich\nSound by Rodion\n\nSaint Petersburg Polytechnic University\nInstitute for Cyber Security\nand Information Protecton\nGroup 4851001/20002\n\n(c) 2023 All Rights reserved");
    sfText_setPosition(Description, (sfVector2f) { 325, 250 });
    sfRenderWindow_drawText(Window, Description, NULL);

    sfText_destroy(Description);
    sfRectangleShape_destroy(Back);
    sfFont_destroy(font);
}

//Процедура отрисовки меню настроек
void CreateSettingsMenuText(sfRenderWindow* Window)
{
    sfRectangleShape* Back = sfRectangleShape_create();
    sfRectangleShape_setSize(Back, (sfVector2f) { 640, 480 });
    sfRectangleShape_setPosition(Back, (sfVector2f) { 320, 200 });
    sfRectangleShape_setFillColor(Back, (sfColor) { 100, 100, 100, 75 });
    sfRenderWindow_drawRectangleShape(Window, Back, NULL);


    sfFont* font = sfFont_createFromFile("BakbakOne-Regular.ttf");
    sfText* Description = sfText_create();
    sfText_setFont(Description, font);
    sfText_setCharacterSize(Description, 50);
    sfText_setString(Description, "Settings");
    sfText_setPosition(Description, (sfVector2f) { 540, 200 });
    sfText_setFillColor(Description, ThemeTosfColor(CurTheme[TextColor]));
    sfRenderWindow_drawText(Window, Description, NULL);

    sfText_setCharacterSize(Description, 40);
    sfText_setString(Description, "Field Size\nBorderless Field\nTheme");
    sfText_setPosition(Description, (sfVector2f) { 350, 300 });
    sfText_setFillColor(Description, ThemeTosfColor(CurTheme[TextColor]));
    sfRenderWindow_drawText(Window, Description, NULL);


    sfText_destroy(Description);
    sfRectangleShape_destroy(Back);
    sfFont_destroy(font);
}
void CreateSettingsMenuButtons(struct Button* SizeButton, struct Button* BorderlessButton, struct Button* ThemeButton)
{
    char text[7] = "128x72";
    if (FieldMode == MEDIUM) strcpy(text, "80x60");
    else if (FieldMode == SMALL)strcpy(text, "64x48");

    CreateButton(SizeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 301.f }, text, ThemeTosfColor(CurTheme[ButtonColor]));

    if (BorderlessStatus == YES) strcpy(text, "On ");
    else if (BorderlessStatus == NO) strcpy(text, "Off");
    CreateButton(BorderlessButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 361.f }, text, ThemeTosfColor(CurTheme[ButtonColor]));

    if (enumTheme == MATRIX) strcpy(text, "Matrix");
    else if (enumTheme == OLD) strcpy(text, "Old");
    CreateButton(ThemeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 421.f }, text, ThemeTosfColor(CurTheme[ButtonColor]));
}
void DrawSettingsButtons(sfRenderWindow* Window, struct Button* SizeButton, struct Button* BorderlessButton, struct Button* ThemeButton)
{
    sfRenderWindow_drawRectangleShape(Window, SizeButton->Shape, NULL);
    sfRenderWindow_drawText(Window, SizeButton->Text, NULL);

    sfRenderWindow_drawRectangleShape(Window, BorderlessButton->Shape, NULL);
    sfRenderWindow_drawText(Window, BorderlessButton->Text, NULL);

    sfRenderWindow_drawRectangleShape(Window, ThemeButton->Shape, NULL);
    sfRenderWindow_drawText(Window, ThemeButton->Text, NULL);

}

//Процедура для создания меню управления игрой
void CreateControlMenu(struct Button* StartStopButton, struct Button* OneStepButton, struct Button* ClearButton, struct Button* RandomButton, struct Button* BackButton)
{
    CreateButton(StartStopButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 180 - 1 }, "Start", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(OneStepButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 272 - 1 }, "One step", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(ClearButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 364 - 1 }, "Clear", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(RandomButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 456 - 1 }, "Random ", ThemeTosfColor(CurTheme[ButtonColor]));
    CreateButton(BackButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 548 - 1 }, "Back", ThemeTosfColor(CurTheme[BackButtonColor]));
}
void DrawControlMenu(sfRenderWindow* Window, struct Button* StartStopButton, struct Button* OneStepButton, struct Button* ClearButton, struct Button* RandomButton, struct Button* BackButton)
{
    sfRenderWindow_drawRectangleShape(Window, StartStopButton->Shape, NULL);
    sfRenderWindow_drawText(Window, StartStopButton->Text, NULL);

    sfRenderWindow_drawRectangleShape(Window, OneStepButton->Shape, NULL);
    sfRenderWindow_drawText(Window, OneStepButton->Text, NULL);

    sfRenderWindow_drawRectangleShape(Window, ClearButton->Shape, NULL);
    sfRenderWindow_drawText(Window, ClearButton->Text, NULL);

    sfRenderWindow_drawRectangleShape(Window, RandomButton->Shape, NULL);
    sfRenderWindow_drawText(Window, RandomButton->Text, NULL);

    sfRenderWindow_drawRectangleShape(Window, BackButton->Shape, NULL);
    sfRenderWindow_drawText(Window, BackButton->Text, NULL);
}

//Удаление кнопки
void DeleteButton(struct Button* Button)
{
    sfRectangleShape_destroy(Button->Shape);
    sfText_destroy(Button->Text);
}

//Создание сетки
void CreateFieldNet(sfVertexArray* FieldNet)
{
    sfVertexArray_clear(FieldNet);
    sfVector2i FieldSize = FieldSizes[FieldMode];

    sfVertex Vert = { (sfVector2f) { 1280 / 2 - FieldSize.x * 7 / 2 - 1, 180 }, ThemeTosfColor(CurTheme[NetColor]),  (sfVector2f) { 0, 0 } };

    for (int i = 0; i < FieldSize.y + 1; i++)
    {
        sfVertexArray_append(FieldNet, Vert);
        Vert.position.x += (FieldSize.x - 1) * 7 + 1 + 7;
        sfVertexArray_append(FieldNet, Vert);
        Vert.position.y += 7;
        Vert.position.x -= (FieldSize.x - 1) * 7 + 1 + 7;
    }
    Vert.position.x = 1280 / 2 - FieldSize.x * 7 / 2;
    Vert.position.y = 180;
    for (int i = 0; i < FieldSize.x + 1; i++)
    {
        sfVertexArray_append(FieldNet, Vert);
        Vert.position.y += (FieldSize.y - 1) * 7 + 7;
        sfVertexArray_append(FieldNet, Vert);
        Vert.position.x += 7;
        Vert.position.y -= (FieldSize.y - 1) * 7 + 7;
    }
}

//Отрисовка живых клеток
void CreateAliveCellsArray(sfVertexArray* AliveCellsArray)
{
    sfVertex Cell = { (sfVector2f) { 1280 / 2 - FieldSizes[FieldMode].x * 7 / 2, 180 },ThemeTosfColor(CurTheme[ButtonColor]),  (sfVector2f) { 0, 0 } };
    for (int i = 0; i < FieldSizes[FieldMode].x; i++)
        for (int j = 0; j < FieldSizes[FieldMode].y; j++)
        {
            if (Field[i][j])
            {
                Cell.position.x = 1280 / 2 - FieldSizes[FieldMode].x * 7 / 2 + i * 7;
                Cell.position.y = 180 + j * 7;
                sfVertexArray_append(AliveCellsArray, Cell);
                Cell.position.x += 6;
                sfVertexArray_append(AliveCellsArray, Cell);
                Cell.position.y += 6;
                sfVertexArray_append(AliveCellsArray, Cell);
                Cell.position.x -= 6;
                sfVertexArray_append(AliveCellsArray, Cell);
                Cell.position.y -= 6;
            }
        }
}

//Обработка клика по полю
int ClickHandle(int ClickX, int ClickY)
{
    int flag = 0;
    if (ClickX >= 1280 / 2 - FieldSizes[FieldMode].x * 7 / 2 && ClickY > 180 - 1)
    {
        int FieldX = (int)(ClickX - (1280 / 2 - FieldSizes[FieldMode].x * 7 / 2)) / 7;
        int FieldY = (int)(ClickY - 180) / 7;
        if (FieldX < FieldSizes[FieldMode].x && FieldY < FieldSizes[FieldMode].y)
        {
            Field[FieldX][FieldY] = !Field[FieldX][FieldY];
            flag = 1;
        }
    }
    return flag;
}

void InitializePaletes(union uPalete OldTheme[5], union uPalete MatrixTheme[5])
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
}

int not_main()
{
    // Создаём окно
    sfVideoMode mode = { WindowSize.x, WindowSize.y, 32 };
    sfRenderWindow* Window = sfRenderWindow_create(mode, "Life the Game v1.2", sfClose, NULL);
    sfImage* Icon = sfImage_createFromFile("Icon.bmp");
    sfRenderWindow_setIcon(Window, 16, 16, sfImage_getPixelsPtr(Icon));
    //done

    FILE* TXTPattern = fopen("MyPatternsList.txt", "r");

    //Начальные условия - игра остановлена, размер поля большой, поле пустое и находимся в главном меню 
    PlayingStatus = STOP;
    FieldMode = LARGE;
    menuStatus = MENU;
    BorderlessStatus = YES;
    enumTheme = MATRIX;
    memset(Field, 0, sizeof(Field));

    union uPalete OldTheme[5];
    union uPalete MatrixTheme[5];
    InitializePaletes(OldTheme, MatrixTheme);

    memcpy(CurTheme, MatrixTheme, sizeof(CurTheme));
    sfRenderWindow_clear(Window, ThemeTosfColor(CurTheme[WallColor]));

    // Создаём кнопки меню
    struct Button PlayButton, SettingsButton, Aboustruct Button, Existruct Button;
    CreateMainMenu(&PlayButton, &SettingsButton, &Aboustruct Button, &Existruct Button);

    //Создаём кнопки управления игровым процессом
    struct Button StartStopButton, OneStepButton, ClearButton, RandomButton, BackButton;
    CreateControlMenu(&StartStopButton, &OneStepButton, &ClearButton, &RandomButton, &BackButton);

    //Создаём кнопки для меню настроек
    struct Button SizeButton, BorderlessButton, ThemeButton;
    CreateSettingsMenuButtons(&SizeButton, &BorderlessButton, &ThemeButton);


    //Создание имени игры сверху окна
    sfText* GameName = sfText_create();
    DrawGameName(GameName);

    //Создание игрового поля - сетки
    sfVertexArray* FieldNet = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(FieldNet, sfLines);
    CreateFieldNet(FieldNet);

    //Массив вершин, содержащий координаты клеток, которые необходимо закрасить - "живых" клеток
    sfVertexArray* AliveCells = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(AliveCells, sfQuads);

    //Шаблоны из файла
    char PatternName[10] = { 0 };
    tPatterns StartPattern;
    memset(&StartPattern, 0, sizeof(StartPattern));
    fscanf(TXTPattern, "%s ", PatternName);
    CreateButton(&StartPattern.Button, (sfVector2f) { 181, 72 }, (sfVector2f) { 1280 - 181, 180 - 1 + 92 * 0 }, PatternName, ThemeTosfColor(CurTheme[ButtonColor]));
    for (int j = 0; j < 64; j++)
        for (int k = 0; k < 48; k++)
            StartPattern.Pattern[j][k] = fgetc(TXTPattern) - 48;
    tPatterns* TmpPattern = &StartPattern;
    for (int i = 1; i < 5; i++)
    {
        TmpPattern->next = calloc(1, sizeof(tPatterns));

        fscanf(TXTPattern, " %s ", PatternName);
        CreateButton(&TmpPattern->next->Button, (sfVector2f) { 181, 72 }, (sfVector2f) { 1280 - 181, 180 - 1 + 92 * i }, PatternName, ThemeTosfColor(CurTheme[ButtonColor]));
        for (int j = 0; j < 64; j++)
            for (int k = 0; k < 48; k++)
                TmpPattern->next->Pattern[j][k] = fgetc(TXTPattern) - 48;
        TmpPattern = TmpPattern->next;
    }
    TmpPattern = &StartPattern;


    sfMusic* music = sfMusic_createFromFile("sound_kuznechik.ogg");
    sfMusic_setLoop(music, sfTrue);
    sfMusic_play(music);

    //Основной цикл игры
    while (sfRenderWindow_isOpen(Window))
    {
        //Проверяем события окошка
        sfEvent event;
        while (sfRenderWindow_pollEvent(Window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(Window);

            else if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {


                //События если находимся в главном меню
                if (menuStatus == MENU)
                {
                    //Если попали по кнопке Play
                    if (sfFloatRect_contains(&PlayButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        //Меняем статус меню и создаём кнопки
                        menuStatus = PLAYING;
                        CreateControlMenu(&StartStopButton, &OneStepButton, &ClearButton, &RandomButton, &BackButton);
                    }
                    //Если попали по кнопке Settings
                    if (sfFloatRect_contains(&SettingsButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        //Настройки
                        menuStatus = SETTINGS;
                        CreateButton(&BackButton, (sfVector2f) { 192, 72 }, (sfVector2f) { 640 - 192 + 320, 480 - 72 + 200 }, "Back", ThemeTosfColor(CurTheme[BackButtonColor]));
                    }
                    //Если попали по кнопке About
                    if (sfFloatRect_contains(&Aboustruct Button.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        //Меняем статус меню на "about" и создаём кнопку "назад"
                        menuStatus = ABOUT;
                        CreateButton(&BackButton, (sfVector2f) { 192, 72 }, (sfVector2f) { 640 - 192 + 320, 480 - 72 + 200 }, "Back", ThemeTosfColor(CurTheme[BackButtonColor]));

                    }
                    //Если попали по кнопке Exit
                    if (sfFloatRect_contains(&Existruct Button.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        //Закрываем окно и завершаем работу программы
                        sfRenderWindow_close(Window);
                    }

                }



                //События если находимся в меню "about" - возможно только событие нажатия на кнопку "назад"
                else if (menuStatus == ABOUT)
                {
                    if (sfFloatRect_contains(&BackButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        menuStatus = MENU;
                    }
                }


                //События пока находимся в меню настроек
                else if (menuStatus == SETTINGS)
                {
                    if (sfFloatRect_contains(&SizeButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {

                        if (FieldMode == LARGE)
                        {
                            CreateButton(&SizeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 301.f }, "64x48", ThemeTosfColor(CurTheme[ButtonColor]));
                            FieldMode = SMALL;
                        }
                        else if (FieldMode == SMALL)
                        {
                            CreateButton(&SizeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 301.f }, "80x60", ThemeTosfColor(CurTheme[ButtonColor]));
                            FieldMode = MEDIUM;
                        }
                        else if (FieldMode == MEDIUM)
                        {
                            CreateButton(&SizeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 301.f }, "128x72", ThemeTosfColor(CurTheme[ButtonColor]));
                            FieldMode = LARGE;
                        }
                        memset(Field, 0, sizeof(Field));
                        CreateFieldNet(FieldNet);
                    }
                    if (sfFloatRect_contains(&BackButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        menuStatus = MENU;
                    }
                    if (sfFloatRect_contains(&BorderlessButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        if (BorderlessStatus == YES)
                        {
                            CreateButton(&BorderlessButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 361.f }, "Off", ThemeTosfColor(CurTheme[BackButtonColor]));
                            BorderlessStatus = NO;
                        }
                        else if (BorderlessStatus == NO)
                        {
                            CreateButton(&BorderlessButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 361.f }, "On ", ThemeTosfColor(CurTheme[ButtonColor]));
                            BorderlessStatus = YES;
                        }
                    }
                    if (sfFloatRect_contains(&ThemeButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        if (enumTheme == MATRIX)
                        {
                            memcpy(CurTheme, OldTheme, sizeof(CurTheme));
                            CreateButton(&ThemeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 421.f }, "Old", ThemeTosfColor(CurTheme[ButtonColor]));
                            enumTheme = OLD;
                        }
                        else
                        {
                            memcpy(CurTheme, MatrixTheme, sizeof(CurTheme));
                            CreateButton(&ThemeButton, (sfVector2f) { 150.f, 50.f }, (sfVector2f) { 800.f, 421.f }, "Matrix", ThemeTosfColor(CurTheme[ButtonColor]));
                            enumTheme = MATRIX;
                        }
                        CreateMainMenu(&PlayButton, &SettingsButton, &Aboustruct Button, &Existruct Button);
                        CreateSettingsMenuButtons(&SizeButton, &BorderlessButton, &ThemeButton);
                        CreateButton(&BackButton, (sfVector2f) { 192, 72 }, (sfVector2f) { 640 - 192 + 320, 480 - 72 + 200 }, "Back", ThemeTosfColor(CurTheme[BackButtonColor]));
                        DrawGameName(GameName);
                        CreateFieldNet(FieldNet);
                        for (int i = 0; TmpPattern != NULL; TmpPattern = TmpPattern->next, i++)
                        {
                            CreateButton(&TmpPattern->Button, (sfVector2f) { 181, 72 }, (sfVector2f) { 1280 - 181, 180 - 1 + 92 * i }, sfText_getString(TmpPattern->Button.Text), ThemeTosfColor(CurTheme[ButtonColor]));
                        }
                        TmpPattern = &StartPattern;


                    }
                }



                //События если находимся на игровом поле
                else if (menuStatus == PLAYING)
                {
                    //Обрабатываем клик, если попали по полчёю, то закрашивается клетка
                    if (ClickHandle(event.mouseButton.x, event.mouseButton.y))
                    {
                        sfVector2i Coor = { (int)(event.mouseMove.x) / 7  , (int)(event.mouseMove.y) / 7 };
                        while (event.type != sfEvtMouseButtonReleased)
                        {
                            sfRenderWindow_clear(Window, ThemeTosfColor(CurTheme[WallColor]));
                            sfRenderWindow_drawText(Window, GameName, NULL);
                            sfRenderWindow_drawVertexArray(Window, FieldNet, NULL);
                            DrawControlMenu(Window, &StartStopButton, &OneStepButton, &ClearButton, &RandomButton, &BackButton);
                            for (; TmpPattern != NULL; TmpPattern = TmpPattern->next)
                            {
                                sfRenderWindow_drawRectangleShape(Window, TmpPattern->Button.Shape, NULL);
                                sfRenderWindow_drawText(Window, TmpPattern->Button.Text, NULL);
                            }
                            TmpPattern = &StartPattern;

                            sfRenderWindow_pollEvent(Window, &event);
                            if (event.type == sfEvtMouseMoved && (Coor.x != (int)(event.mouseMove.x) / 7 || Coor.y != (int)(event.mouseMove.y) / 7))
                            {
                                ClickHandle(event.mouseMove.x, event.mouseMove.y);
                                CreateAliveCellsArray(AliveCells);
                                sfRenderWindow_drawVertexArray(Window, AliveCells, NULL);
                            }

                            if (PlayingStatus == GO)
                            {
                                NewGeneration();
                                Sleep(30);
                            }
                            CreateAliveCellsArray(AliveCells);
                            sfRenderWindow_drawVertexArray(Window, AliveCells, NULL);
                            sfRenderWindow_display(Window);
                            sfVertexArray_clear(AliveCells);
                            Coor.x = (int)(event.mouseMove.x) / 7;
                            Coor.y = (int)(event.mouseMove.y) / 7;
                        }
                    }
                    //Обрабатываем попадания по различным кнопкам

                    //Кнопка старт/стоп - меняет цвет и надпись и запускает и останавливает игру
                    if (sfFloatRect_contains(&StartStopButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        if (PlayingStatus == GO)
                        {
                            CreateButton(&StartStopButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 180 - 1 }, "Start", ThemeTosfColor(CurTheme[ButtonColor]));
                        }
                        else
                        {
                            CreateButton(&StartStopButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 180 - 1 }, "Stop", ThemeTosfColor(CurTheme[BackButtonColor]));
                        }
                        PlayingStatus = !PlayingStatus;
                    }

                    //Кнопка one step делает один шаг 
                    if (sfFloatRect_contains(&OneStepButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        if (PlayingStatus == GO)
                        {
                            PlayingStatus = !PlayingStatus;
                            CreateButton(&StartStopButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 180 - 1 }, "Start", ThemeTosfColor(CurTheme[ButtonColor]));
                        }
                        NewGeneration();
                    }

                    if (sfFloatRect_contains(&ClearButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        if (PlayingStatus == GO)
                        {
                            PlayingStatus = !PlayingStatus;
                            CreateButton(&StartStopButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 180 - 1 }, "Start", ThemeTosfColor(CurTheme[ButtonColor]));
                        }
                        memset(Field, 0, sizeof(Field));

                    }
                    if (sfFloatRect_contains(&RandomButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        srand(time(NULL));
                        for (int i = 0; i < FieldSizes[FieldMode].x; i++)
                            for (int j = 0; j < FieldSizes[FieldMode].y; j++)
                            {
                                Field[i][j] = rand() % 2;
                            }
                    }

                    for (; TmpPattern != NULL; TmpPattern = TmpPattern->next)
                    {
                        if (sfFloatRect_contains(&TmpPattern->Button.Bound, event.mouseButton.x, event.mouseButton.y))
                        {
                            memset(Field, 0, sizeof(Field));
                            for (int j = 0; j < 64; j++)
                                for (int k = 0; k < 48; k++)
                                    Field[j][k] = TmpPattern->Pattern[j][k];
                        }
                    }
                    TmpPattern = &StartPattern;

                    if (sfFloatRect_contains(&BackButton.Bound, event.mouseButton.x, event.mouseButton.y))
                    {
                        if (PlayingStatus == GO)
                        {
                            PlayingStatus = !PlayingStatus;
                            CreateButton(&StartStopButton, (sfVector2f) { 181, 72 }, (sfVector2f) { 0, 180 - 1 }, "Start", ThemeTosfColor(CurTheme[ButtonColor]));
                        }

                        menuStatus = MENU;
                    }
                }
            }
        }



        //Очищаем поле перед рисованием
        sfRenderWindow_clear(Window, ThemeTosfColor(CurTheme[WallColor]));

        //Помещаем надпись сверху на экран
        sfRenderWindow_drawText(Window, GameName, NULL);

        //Действия если находимся в каком-либо из меню
        if (menuStatus == MENU)
        {
            DrawMainMenu(Window, &PlayButton, &SettingsButton, &Aboustruct Button, &Existruct Button);
        }
        else if (menuStatus == ABOUT)
        {
            CreateAboutMenu(Window);
            sfRenderWindow_drawRectangleShape(Window, BackButton.Shape, NULL);
            sfRenderWindow_drawText(Window, BackButton.Text, NULL);
        }
        else if (menuStatus == SETTINGS)
        {
            CreateSettingsMenuText(Window);
            sfRenderWindow_drawRectangleShape(Window, BackButton.Shape, NULL);
            sfRenderWindow_drawText(Window, BackButton.Text, NULL);
            DrawSettingsButtons(Window, &SizeButton, &BorderlessButton, &ThemeButton);

        }
        else if (menuStatus == PLAYING)
        {
            //Отрисовка игрового поля

            //Рисуем сетку
            sfRenderWindow_drawVertexArray(Window, FieldNet, NULL);
            //Заполняем массив вершинами, исходя из того, какие клетки живы
            CreateAliveCellsArray(AliveCells);
            //Рисуем живые клетки
            sfRenderWindow_drawVertexArray(Window, AliveCells, NULL);

            //Отрисвка всех кнопок
            DrawControlMenu(Window, &StartStopButton, &OneStepButton, &ClearButton, &RandomButton, &BackButton);

            //СДЕЛАТЬ ШАБЛОНЫ
            for (; TmpPattern != NULL; TmpPattern = TmpPattern->next)
            {
                sfRenderWindow_drawRectangleShape(Window, TmpPattern->Button.Shape, NULL);
                sfRenderWindow_drawText(Window, TmpPattern->Button.Text, NULL);
            }
            TmpPattern = &StartPattern;

            //Если процесс запущен, то создаём новое поколение клеток с перерывом 50мс
            if (PlayingStatus == GO)
            {
                NewGeneration();
                Sleep(30);
            }
        }

        //Очищаем массив вершин
        sfVertexArray_clear(AliveCells);

        //Отображаем все изменения на поле
        sfRenderWindow_display(Window);
    }

    // Освобождаем память и закрываем окно
    DeleteButton(&PlayButton);
    DeleteButton(&Aboustruct Button);
    DeleteButton(&Existruct Button);
    DeleteButton(&BackButton);
    DeleteButton(&StartStopButton);
    DeleteButton(&OneStepButton);
    DeleteButton(&ClearButton);
    DeleteButton(&RandomButton);
    DeleteButton(&SizeButton);
    DeleteButton(&BorderlessButton);
    DeleteButton(&ThemeButton);

    sfVertexArray_destroy(FieldNet);
    sfVertexArray_destroy(AliveCells);

    DeleteButton(&StartPattern.Button);
    TmpPattern = StartPattern.next;
    for (; TmpPattern != NULL; TmpPattern = TmpPattern->next)
    {
        DeleteButton(&TmpPattern->Button);
        free(TmpPattern);
    }

    sfImage_destroy(Icon);
    sfRenderWindow_destroy(Window);
    sfMusic_destroy(music);
    return 0;
}
