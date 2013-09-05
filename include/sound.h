#ifndef SOUND_H
#define SOUND_H

#include <SDL/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct Sound {
	int position;
	Uint8 *buffer;
	int size;
} Sound;

extern void callback(void *userdata, Uint8 *stream, int len);

extern ostream& operator<<(ostream& os, const SDL_AudioSpec& spec);

#endif