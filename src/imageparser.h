#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "types.h"

struct Image {
	int width;
	int height;
	uint8 *pixels;
};

class ImageParser {
public:
	Image ParseImage(const char *file, int scalefactor);
	
private:
	SDL_Surface* OpenImage(const char *file);

	Image ScaleDownSurface(SDL_Surface *surface, int factor);
	uint8 GetGrayscalePixel(int x, int y, SDL_Surface*);
	uint8 GetColor(int x, int y, SDL_Surface *surface, unsigned mask);
};
