#include "window.h"



void windowCreate()
{
	sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, BITS_PER_PIXEL };
	window = sfRenderWindow_create(mode, "Life the Game v1.2", sfClose, NULL);
	sfImage* Icon = sfImage_createFromFile("Icon.bmp");
	sfRenderWindow_setIcon(window, 16, 16, sfImage_getPixelsPtr(Icon));
}