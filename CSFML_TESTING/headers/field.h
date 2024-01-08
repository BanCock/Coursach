#ifndef FIELD_H
#define FIELD_H

#include <SFML/Graphics.h>
#include <malloc.h>

#include "interface.h"

char* cells;

struct field
{
	sfVertexArray* fieldNet;
	sfVertexArray* aliveCells;
} field;

void createField();
void fillAliveCells();
void recreateFieldNet();
void nextGen();
void fieldUpdate();

void cleanField();

#endif // !FIELD_H
