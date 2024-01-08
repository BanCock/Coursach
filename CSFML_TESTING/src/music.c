#include "../headers/music.h"

static sfMusic* music;

void setMusic()
{
    music = sfMusic_createFromFile("sound_kuznechik.ogg");
    sfMusic_setLoop(music, sfTrue);
    sfMusic_play(music);
}

void cleanMusic()
{
    sfMusic_destroy(music);
}

