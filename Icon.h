#pragma once
#include <SDL.h>
#include <string>

#include "Element.h"

class Icon : public Element
{
public:
	Icon() {}
	Icon(SDL_Renderer* renderer, int x, int y, string src);
private:

	SDL_Surface* img  = nullptr;
	SDL_Texture* text = nullptr;


};

