#include <SDL.h>

#include "Util.h"
#include "Globals.h"
#include "Constants.h"
#include "LTexture.h"


extern LTexture gSpriteSheetTexture;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia(std::string path)
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if (!(gSpriteSheetTexture.loadFromFile(path)))
	{
		printf("Failed to load chess pieces sprite sheet texture!\n");
		success = false;
	}
	
	/////////////////////////
	// WHITE PAWN
	gSpriteClips[0].x = 0;
	gSpriteClips[0].y = 200;
	gSpriteClips[0].w = 40;
	gSpriteClips[0].h = 40;

	// WHITE HORSE
	gSpriteClips[1].x = 0;
	gSpriteClips[1].y = 120;
	gSpriteClips[1].w = 40;
	gSpriteClips[1].h = 40;

	// WHITE BISHOP
	gSpriteClips[2].x = 0;
	gSpriteClips[2].y = 80;
	gSpriteClips[2].w = 40;
	gSpriteClips[2].h = 40;

	// WHITE ROOK
	gSpriteClips[3].x = 0;
	gSpriteClips[3].y = 160;
	gSpriteClips[3].w = 40;
	gSpriteClips[3].h = 40;

	// WHITE QUEEN
	gSpriteClips[4].x = 0;
	gSpriteClips[4].y = 0;
	gSpriteClips[4].w = 40;
	gSpriteClips[4].h = 40;

	// WHITE KING
	
	gSpriteClips[5].x = 0;
	gSpriteClips[5].y = 40;
	gSpriteClips[5].w = 40;
	gSpriteClips[5].h = 40;

	/////////////////////////
	// BLACK PAWN
	gSpriteClips[6].x = 40;
	gSpriteClips[6].y = 200;
	gSpriteClips[6].w = 40;
	gSpriteClips[6].h = 40;

	// BLACK KNIGHT
	gSpriteClips[7].x = 40;
	gSpriteClips[7].y = 120;
	gSpriteClips[7].w = 40;
	gSpriteClips[7].h = 40;

	// BLACK BISHOP
	gSpriteClips[8].x = 40;
	gSpriteClips[8].y = 80;
	gSpriteClips[8].w = 40;
	gSpriteClips[8].h = 40;
	// BLACK ROOK
	gSpriteClips[9].x = 40;
	gSpriteClips[9].y = 160;
	gSpriteClips[9].w = 40;
	gSpriteClips[9].h = 40;

	// BLACK QUEEN
	gSpriteClips[10].x = 40;
	gSpriteClips[10].y = 0;
	gSpriteClips[10].w = 40;
	gSpriteClips[10].h = 40;

	// BLACK KING
	gSpriteClips[11].x = 40;
	gSpriteClips[11].y = 40;
	gSpriteClips[11].w = 40;
	gSpriteClips[11].h = 40;

	return success;
}

void close()
{

	//Destroy window    
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}