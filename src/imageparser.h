#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


struct Image {
	int width;
	int height;
	int length;
	unsigned char *pixels;
};

class ImageParser {
public:
	Image ParseImage(const char *file);
	
private:
	SDL_Surface* OpenImage(const char *file);
};
