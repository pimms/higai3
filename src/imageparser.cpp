#include "imageparser.h"
#include <assert.h>

Image ImageParser::ParseImage(const char *file, int scalefactor)
{
	printf("Parsing image: %s...", file);

	SDL_Surface *surface = OpenImage(file);

	if (!surface) {
		throw runtime_error("Failed to open image");
		return Image{0, 0, NULL};
	}
	
	Image img = ScaleDownSurface(surface, scalefactor);

	printf("ok\n");
	return img;
}


/* Private Methods */

SDL_Surface* ImageParser::OpenImage(const char *file)
{
	SDL_Surface *surface = IMG_Load(file);
	return surface;
}

Image ImageParser::ScaleDownSurface(SDL_Surface *surface, int factor)
{
	assert((surface->w % factor) == 0);
	assert((surface->h % factor) == 0);

	Image img;
	img.width = surface->w / factor;
	img.height = surface->h / factor;
	img.pixels = new uint8[img.width * img.height];

	for (int x=0; x<surface->w; x += factor) {
		for (int y=0; y<surface->h; y += factor) {
			unsigned avg = 0;

			for (int xi=0; xi<factor; xi++) {
				for (int yi=0; yi<factor; yi++) {
					avg += GetGrayscalePixel(x+xi, y+yi, surface);
				}
			}

			avg /= (factor*factor);

			int idx = (y/factor) * img.width + (x/factor);
			img.pixels[idx] = (uint8)avg;
		}
	}

	return img;
}

uint8 ImageParser::GetGrayscalePixel(int x, int y, SDL_Surface *surface)
{
	SDL_PixelFormat *pf = surface->format;

	unsigned gray;

	if (pf->BytesPerPixel >= 3) {
		uint8 r, g, b;
		r = GetColor(x, y, surface, pf->Rmask);
		g = GetColor(x, y, surface, pf->Gmask);
		b = GetColor(x, y, surface, pf->Bmask);

		gray = (r + g + b) / 3;
	} else {
		throw runtime_error("3 or 4 bytes per pixel please");
	}

	return (uint8)gray;
}

uint8 ImageParser::GetColor(int x, int y, SDL_Surface *surf, 
							unsigned mask)
{
	SDL_PixelFormat *fm = surf->format;
	uint8 *bm = (uint8*)&mask;

	for (int i=0; i<fm->BytesPerPixel; i++) {
		if (bm[i] == 255) {
			uint8 *pix = (uint8*)surf->pixels;
			pix += (y * surf->pitch) + (x * fm->BytesPerPixel);

			return *pix;
		} else if (bm[i] != 0) {
			throw runtime_error("Weirdly aligned mask yo");
		}
	}

	return 0;
}
