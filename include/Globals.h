#ifndef GLOBALS_H
#define GLOBALS_H

#include "LTexture.h"

#include <SDL.h>

// The window we'll be rendering to
extern SDL_Window* gWindow;

// The window renderer
extern SDL_Renderer* gRenderer;

// Scene sprites
extern SDL_Rect gSpriteClips[16];
extern LTexture gSpriteSheetTexture;

// Game loop condition
extern bool quit;
#endif 