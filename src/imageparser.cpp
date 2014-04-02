#include "imageparser.h"

Image ImageParser::ParseImage(const char *file)
{
	Image img {0, 0, 0, NULL};
	SDL_Surface *surface = OpenImage(file);

	if (!surface) {
		return img;
	}
	
	// TODO
	// Resize the image

	return img;
}


/* Private Methods */

SDL_Surface* ImageParser::OpenImage(const char *file)
{
	SDL_Surface *surface = IMG_Load(file);
	return surface;
}
