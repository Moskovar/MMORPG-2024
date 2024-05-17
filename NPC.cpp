#include "NPC.h"

NPC::NPC(std::string name, float x, float y, int category, SDL_Window* window, SDL_Renderer* renderer, string className) : Entity(name, x, y, category, window, renderer, className)
{
	clickBox.x = x + 90;
	clickBox.y = y + 65;
	clickBox.w = 75;
	clickBox.h = 125;
}

void NPC::draw(SDL_Renderer* renderer)
{
	pseudo.draw(renderer, getPseudoX(), getPseudoY());
	SDL_RenderCopy(renderer, text[animationID][dir][step], NULL, &this->pos);
}
