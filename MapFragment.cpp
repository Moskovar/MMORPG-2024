#include "MapFragment.h"
#include "uti.h"

MapFragment::MapFragment(string src, SDL_Renderer* renderer)
{
	img = IMG_Load(string("img/mapfragments/" + src + ".png").c_str());
	//uti::loadTexture(text, img, renderer, "MapFragment -> " + src);
	if (img) { text = SDL_CreateTextureFromSurface(renderer, img); }
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: mapfragment").c_str()); exit(0); }

	pos.w = 1920;
	pos.h = 1080;
}
