#include "patterns.h"

void loadPatterns()
{
    FILE* filePattern = fopen("MyPatternsList.txt", "r");
    char patternName[10] = "";
    sfVector2f position =
    {
        1280 - 181, \
        180 - 1
    };
    

    memset(&startPattern, 0, sizeof(startPattern));
    fscanf(filePattern, "%s ", patternName);
    createButton(&startPattern.Button, CONTROLMENU_BUTTON_SIZE, position, patternName);

    for (int j = 0; j < 64; j++)
        for (int k = 0; k < 48; k++)
            startPattern.Pattern[j][k] = fgetc(filePattern) - 48;

    struct sPatterns* tmpPattern = &startPattern;
    for (int i = 1; i < 5; i++)
    {
        tmpPattern->next = calloc(1, sizeof(struct sPatterns));
        position.y += 92;

        fscanf(filePattern, " %s ", patternName);
        createButton(&tmpPattern->next->Button, CONTROLMENU_BUTTON_SIZE, position, patternName);

        for (int j = 0; j < 64; j++)
            for (int k = 0; k < 48; k++)
                tmpPattern->next->Pattern[j][k] = fgetc(filePattern) - 48;

        tmpPattern = tmpPattern->next;
    }
}