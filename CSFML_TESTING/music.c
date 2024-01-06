#include "music.h"

void setMusic()
{
    sfMusic* music = sfMusic_createFromFile("sound_kuznechik.ogg");
    sfMusic_setLoop(music, sfTrue);
    sfMusic_play(music);
}