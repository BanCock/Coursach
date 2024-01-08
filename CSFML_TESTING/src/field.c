#include "../headers/field.h"

static sfVector2i fieldSizes[3] = { {64, 48}, {80, 60}, {128, 72} };

static char* cellsBuf;

static void nextGenBlYes();
static void nextGenBlNo();

void createField()
{
	cells = calloc(128u * 72u, sizeof(char));
	cellsBuf = calloc(128u * 72u, sizeof(char));

	field.fieldNet = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(field.fieldNet, sfLines);
	sfVertexArray_clear(field.fieldNet);

	field.aliveCells = sfVertexArray_create();
	sfVertexArray_setPrimitiveType(field.aliveCells, sfQuads);

	sfVector2i fieldSize = fieldSizes[FieldMode];
	sfVertex vert = { (sfVector2f) { 640 - fieldSize.x * 7 / 2 - 1, 180.f },\
					ThemeTosfColor(CurTheme[NetColor]), \
					(sfVector2f) { 0, 0 } };

	for (int i = 0; i < fieldSize.y + 1; i++)
	{
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.x += (fieldSize.x - 1) * 7 + 1 + 7;
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.y += 7;
		vert.position.x -= (fieldSize.x - 1) * 7 + 1 + 7;
	}

	vert.position.x = 640 - fieldSize.x * 7 / 2;
	vert.position.y = 180;

	for (int i = 0; i < fieldSize.x + 1; i++)
	{
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.y += (fieldSize.y - 1) * 7 + 7;
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.x += 7;
		vert.position.y -= (fieldSize.y - 1) * 7 + 7;
	}
}

void fillAliveCells()
{
	sfVertexArray_clear(field.aliveCells);

	sfVertex Cell = { (sfVector2f) { 640 - fieldSizes[FieldMode].x * 7 / 2, 180 },ThemeTosfColor(CurTheme[ButtonColor]),  (sfVector2f) { 0, 0 } };
	for (int i = 0; i < fieldSizes[FieldMode].x; i++)
		for (int j = 0; j < fieldSizes[FieldMode].y; j++)
		{
			if (cells[i + j * fieldSizes[FieldMode].x])
			{
				Cell.position.x = 640 - fieldSizes[FieldMode].x * 7 / 2 + i * 7;
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

void recreateFieldNet()
{
	sfVertexArray_clear(field.fieldNet);
	sfVector2i fieldSize = fieldSizes[FieldMode];
	sfVertex vert = { (sfVector2f) { 640 - fieldSize.x * 7 / 2 - 1, 180.f },\
					ThemeTosfColor(CurTheme[NetColor]), \
					(sfVector2f) { 0, 0 } };

	for (int i = 0; i < fieldSize.y + 1; i++)
	{
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.x += (fieldSize.x - 1) * 7 + 1 + 7;
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.y += 7;
		vert.position.x -= (fieldSize.x - 1) * 7 + 1 + 7;
	}

	vert.position.x = 640 - fieldSize.x * 7 / 2;
	vert.position.y = 180;

	for (int i = 0; i < fieldSize.x + 1; i++)
	{
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.y += (fieldSize.y - 1) * 7 + 7;
		sfVertexArray_append(field.fieldNet, vert);
		vert.position.x += 7;
		vert.position.y -= (fieldSize.y - 1) * 7 + 7;
	}
}

void nextGen()
{
	switch (BorderlessStatus)
	{
	case YES:
		nextGenBlYes();
		break;
	case NO:
		nextGenBlNo();
		break;

	}
}

void fieldUpdate()
{
	switch (PlayingStatus)
	{
	case GO:
		nextGen();
		break;
	}
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
	for (int x = 0; x < fieldSizes[FieldMode].x; x++)
		for (int y = 0; y < fieldSizes[FieldMode].y; y++)
		{
			int LeftSide = (x - 1 < 0) ? fieldSizes[FieldMode].x - 1 : x - 1;
			int RightSide = (x + 1 > fieldSizes[FieldMode].x - 1) ? 0 : x + 1;
			int UpSide = (y - 1 < 0) ? fieldSizes[FieldMode].y - 1 : y - 1;
			int DownSide = (y + 1 > fieldSizes[FieldMode].y - 1) ? 0 : y + 1;

			int AliveCell = cells[UpSide * fieldSizes[FieldMode].x + LeftSide] + cells[UpSide * fieldSizes[FieldMode].x + x] + \
				cells[UpSide * fieldSizes[FieldMode].x + RightSide] + cells[y * fieldSizes[FieldMode].x + LeftSide] + \
				cells[y * fieldSizes[FieldMode].x + RightSide] + cells[DownSide * fieldSizes[FieldMode].x + LeftSide] + \
				cells[DownSide * fieldSizes[FieldMode].x + x] + cells[DownSide * fieldSizes[FieldMode].x + RightSide];

			cellsBuf[y * fieldSizes[FieldMode].x + x] = (AliveCell == 3 || (AliveCell == 2 && cells[y * fieldSizes[FieldMode].x + x])) ? 1 : 0;
		}
	char* tmp = cells;
	cells = cellsBuf;
	cellsBuf = tmp;

}

void nextGenBlNo()
{
	for (int x = 0; x < fieldSizes[FieldMode].x; x++)
		for (int y = 0; y < fieldSizes[FieldMode].y; y++)
		{
			int LeftSide = x - 1;
			int RightSide = x + 1;
			int UpSide = y - 1;
			int DownSide = y + 1;

			int AliveCell = (LeftSide >= 0) * (UpSide >= 0) * cells[UpSide * fieldSizes[FieldMode].x + LeftSide] + \
							(UpSide >= 0) * cells[UpSide * fieldSizes[FieldMode].x + x] + \
							(UpSide >= 0) * (RightSide <= fieldSizes[FieldMode].x) * cells[UpSide * fieldSizes[FieldMode].x + RightSide] + \
							(LeftSide >= 0) * cells[y * fieldSizes[FieldMode].x + LeftSide] + \
							(RightSide <= fieldSizes[FieldMode].x) * cells[y * fieldSizes[FieldMode].x + RightSide] + \
							(DownSide <= fieldSizes[FieldMode].y) * (LeftSide >= 0) * cells[DownSide * fieldSizes[FieldMode].x + LeftSide] + \
							(DownSide <= fieldSizes[FieldMode].y) * cells[DownSide * fieldSizes[FieldMode].x + x] + \
							(DownSide <= fieldSizes[FieldMode].y) * (RightSide <= fieldSizes[FieldMode].x) * cells[DownSide * fieldSizes[FieldMode].x + RightSide];

			cellsBuf[y * fieldSizes[FieldMode].x + x] = (AliveCell == 3 || (AliveCell == 2 && cells[y * fieldSizes[FieldMode].x + x])) ? 1 : 0;
		}
	char* tmp = cells;
	cells = cellsBuf;
	cellsBuf = tmp;
}
