#include "Globals.h"
#include "LTexture.h"
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Scene sprites
SDL_Rect gSpriteClips[16];
LTexture gSpriteSheetTexture;

// Game loop condition
bool quit = false;


