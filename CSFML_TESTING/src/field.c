#include "../headers/field.h"

static sfVector2i fieldSizes[3] = { {64, 48}, {80, 60}, {128, 72} };

static char* cellsBuf;

static void nextGenBlYes();
static void nextGenBlNo();

void createField()
{
	cells = calloc((fieldSizes[LARGE].x + 2) * (fieldSizes[LARGE].y + 2), sizeof(char));
	cellsBuf = calloc((fieldSizes[LARGE].x + 2) * (fieldSizes[LARGE].y + 2), sizeof(char));

	field.fieldNet = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(field.fieldNet, sfLineStrip);
	sfVertexArray_clear(field.fieldNet);

	field.aliveCells = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(field.aliveCells, sfQuads);

	sfVector2i fieldSize = fieldSizes[FieldMode];
	sfVertex vert = { (sfVector2f) { 640 - fieldSize.x * 7 / 2 - 1, 180.f },\
					ThemeTosfColor(CurTheme[NetColor]), \
					(sfVector2f) { 0, 0 } };

	/*for (int i = 0; i < fieldSize.y + 1; i++)
	{
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.x += (fieldSize.x - 1) * 7 + 1 + 7;
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.y += 7;
		vert.position.x -= (fieldSize.x - 1) * 7 + 1 + 7;
	}*/
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.x += (fieldSize.x - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.y += (fieldSize.y - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.x -= (fieldSize.x - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.y -= (fieldSize.y - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);

	/*vert.position.x = 640 - fieldSize.x * 7 / 2;
	vert.position.y = 180;

	for (int i = 0; i < fieldSize.x + 1; i++)
	{
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.y += (fieldSize.y - 1) * 7 + 7;
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.x += 7;
		vert.position.y -= (fieldSize.y - 1) * 7 + 7;
	}*/
}

void fillAliveCells()
{
	sfVertexArray_clear(field.aliveCells);

	sfVertex Cell = { (sfVector2f) { 640 - fieldSizes[FieldMode].x * 7 / 2, 180 },ThemeTosfColor(CurTheme[ButtonColor]),  (sfVector2f) { 0, 0 } };
	for (int i = 0; i < fieldSizes[FieldMode].x; i++)
		for (int j = 0; j < fieldSizes[FieldMode].y; j++)
		{
			if (cells[1 + i + (j + 1) * (fieldSizes[FieldMode].x + 2)])
			{
				Cell.position.x = 640 - (fieldSizes[FieldMode].x) * 7 / 2 + i * 7;
				Cell.position.y = 180 + j * 7;
				sfVertexArray_append(field.aliveCells, Cell);
				Cell.position.x += 6;
				sfVertexArray_append(field.aliveCells, Cell);
				Cell.position.y += 6;
				sfVertexArray_append(field.aliveCells, Cell);
				Cell.position.x -= 6;
				sfVertexArray_append(field.aliveCells, Cell);
				Cell.position.y -= 6;
			}
		}
}

void recreatrFieldNet()
{
	sfVertexArray_clear(field.fieldNet);
	sfVector2i fieldSize = fieldSizes[FieldMode];
	sfVertex vert = { (sfVector2f) { 640 - fieldSize.x * 7 / 2 - 1, 180.f },\
					ThemeTosfColor(CurTheme[NetColor]), \
					(sfVector2f) { 0, 0 } };

	sfVertexArray_append(field.fieldNet, vert);
	vert.position.x += (fieldSize.x - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.y += (fieldSize.y - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.x -= (fieldSize.x - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);
	vert.position.y -= (fieldSize.y - 1) * 7 + 1 + 7;
	sfVertexArray_append(field.fieldNet, vert);

}

void nextGen()
{
	frequencyCounter.counter++;
#ifndef _VOID_GAME
	switch (BorderlessStatus)
	{
	case NO:
		nextGenBlNo();
		break;
	case YES:
		nextGenBlYes();
		break;
	}
#endif // !_VOID_GAME

	
}

void fieldUpdate()
{
	if (!PlayingStatus) nextGen();
}

void cleanField()
{
	sfVertexArray_destroy(field.fieldNet);
	sfVertexArray_destroy(field.aliveCells);

	free(cells);
	free(cellsBuf);
}

static void nextGenBlYes()
{
	for (int x = 1; x < fieldSizes[FieldMode].x + 1; x++)
		for (int y = 1; y < fieldSizes[FieldMode].y + 1; y++)
		{
			int LeftSide = (x - 1 < 1) ? fieldSizes[FieldMode].x : x - 1;
			int RightSide = (x + 1 > fieldSizes[FieldMode].x) ? 1 : x + 1;
			int UpSide = (y - 1 < 1) ? fieldSizes[FieldMode].y : y - 1;
			int DownSide = (y + 1 > fieldSizes[FieldMode].y) ? 1 : y + 1;

			int AliveCell = cells[UpSide * (fieldSizes[FieldMode].x + 2) + LeftSide] + cells[UpSide * (fieldSizes[FieldMode].x + 2) + x] + \
				cells[UpSide * (fieldSizes[FieldMode].x + 2) + RightSide] + cells[y * (fieldSizes[FieldMode].x + 2) + LeftSide] + \
				cells[y * (fieldSizes[FieldMode].x + 2) + RightSide] + cells[DownSide * (fieldSizes[FieldMode].x + 2) + LeftSide] + \
				cells[DownSide * (fieldSizes[FieldMode].x + 2) + x] + cells[DownSide * (fieldSizes[FieldMode].x + 2) + RightSide];

			cellsBuf[y * (fieldSizes[FieldMode].x + 2) + x] = (AliveCell == 3 || (AliveCell == 2 && cells[y * (fieldSizes[FieldMode].x + 2) + x])) ? 1 : 0;
		}
	char* tmp = cells;
	cells = cellsBuf;
	cellsBuf = tmp;

}

void nextGenBlNo()
{
	unsigned long long* ullcellsBuf;
	int left, right, up, down;
	char table[16] = { 0 };
	table[3] = table[10] = table[11] = 1;
	for (int y = 1; y < fieldSizes[FieldMode].y; y++)
		for (int x = 1; x < fieldSizes[FieldMode].x; x += 8)
		{
			left = x - 1;
			right = x + 1;
			up = y - 1;
			down = y + 1;
			ullcellsBuf = (unsigned long long*)(cellsBuf + y * (fieldSizes[FieldMode].x + 2) + x);
			*ullcellsBuf = *(unsigned long long*)(cells + up * (fieldSizes[FieldMode].x + 2) + left) + \
				* (unsigned long long*)(cells + up * (fieldSizes[FieldMode].x + 2) + x) + \
				* (unsigned long long*)(cells + up * (fieldSizes[FieldMode].x + 2) + right) + \
				* (unsigned long long*)(cells + y * (fieldSizes[FieldMode].x + 2) + left) + \
				* (unsigned long long*)(cells + y * (fieldSizes[FieldMode].x + 2) + right) + \
				* (unsigned long long*)(cells + down * (fieldSizes[FieldMode].x + 2) + left) + \
				* (unsigned long long*)(cells + down * (fieldSizes[FieldMode].x + 2) + x) + \
				* (unsigned long long*)(cells + down * (fieldSizes[FieldMode].x + 2) + right);
		}
	unsigned long long alive, neighbours, an, mnl;
	int index; 
	for (int y = 1; y < fieldSizes[FieldMode].y + 1; y++)
		for (int x = 1; x < fieldSizes[FieldMode].x + 1; x+=8)
		{
			index = y * (fieldSizes[FieldMode].x + 2) + x;
			/*neighbours = ((cellsBuf[index] & 0b111) | (cells[index] << 3));
			cells[index] = table[neighbors];*/
			neighbours = *(unsigned long long*)(cellsBuf + index);
			alive = *(unsigned long long*)(cells + index);
			neighbours &= 0b0000011100000111000001110000011100000111000001110000011100000111ul;

			an = ((neighbours & ~0b0000000100000001000000010000000100000001000000010000000100000001ul) >> 1) | (alive << 2);
			an ^= ~0b0000010100000101000001010000010100000101000001010000010100000101ul;
			an &= (an >> 2);
			an &= (an >> 1);
			an &= 0b0000000100000001000000010000000100000001000000010000000100000001ul;

			mnl = neighbours | (alive << 3);
			mnl ^= ~0b0000001100000011000000110000001100000011000000110000001100000011ul;
			mnl &= (mnl >> 2);
			mnl &= (mnl >> 1);
			mnl &= 0b0000000100000001000000010000000100000001000000010000000100000001ul;

			*(unsigned long long*)(cells + index) = an | mnl;
		}
}
