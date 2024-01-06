#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.h>

#define WINDOW_WIDTH		1280
#define WINDOW_HEIGHT		720
#define BITS_PER_PIXEL		32

sfRenderWindow* window;

void windowCreate();

#endif // !WINDOW_H

