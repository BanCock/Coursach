#ifndef PATTERNS_H
#define PATTERNS_H

#include <stdio.h>

#include "interface.h"

void loadPatterns();

struct sPatterns startPattern;

struct sPatterns
{
    int Pattern[64][48];
    struct Button Button;
    struct sPatterns* next;
};



#endif // !PATTERNS_H

