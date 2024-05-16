#pragma once
#include "Element.h"
class Building : public Element
{
public:
	Building() {}
	Building(float x, float y, int width, int height, SDL_Renderer* renderer, string src);

	bool isInFront(int x, int y);
	bool check_collisions(int x, int y);

	virtual void draw(SDL_Renderer* renderer);
private:
	SDL_Surface* img   = nullptr;
	SDL_Surface* imgBg = nullptr;
	SDL_Texture* text  = nullptr;

	bool wasInFront = false;
};

