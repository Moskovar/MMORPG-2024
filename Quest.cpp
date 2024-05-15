#include "Quest.h"

Quest::Quest(string name, string desc, int exp, SDL_Window* window, SDL_Renderer* renderer)
{
	this->name = Font(name, 14, window, renderer);
	this->desc = Font(desc, 14, window, renderer);
	this->exp  = exp;
}

void Quest::drawName(SDL_Renderer* renderer, int x, int y)
{
	name.draw(renderer, x, y);
}

void Quest::drawDesc(SDL_Renderer* renderer, int x, int y)
{
	desc.draw(renderer, x, y);
}
