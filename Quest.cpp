#include "Quest.h"

Quest::Quest(string name, string desc, int exp, SDL_Renderer* renderer)
{
	this->name = Font(name, 14, 255, 255, 255, 255, renderer);
	this->desc = Font(desc, 14, 255, 255, 255, 255, renderer);
	this->exp  = exp;
	iconPos = { 0, 0, 17, 60 };
}

void Quest::drawName(SDL_Renderer* renderer, int x, int y)
{
	name.draw(renderer, x, y);
}

void Quest::drawDesc(SDL_Renderer* renderer, int x, int y)
{
	desc.draw(renderer, x, y);
}
