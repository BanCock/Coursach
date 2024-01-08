#include "../headers/window.h"

static sfImage* Icon;

void windowCreate()
{
	sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, BITS_PER_PIXEL };
	window = sfRenderWindow_create(mode, "Life the Game v1.2", sfClose, NULL);
	Icon = sfImage_createFromFile("Icon.bmp");
	sfRenderWindow_setIcon(window, 16, 16, sfImage_getPixelsPtr(Icon));
}

void cleanWindow()
{
	sfImage_destroy(Icon);
	sfRenderWindow_destroy(window);
}

