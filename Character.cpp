#include "Character.h"

Character::Character(std::string name, int category, SDL_Window* window, SDL_Renderer* renderer, string src) : Entity(name, category, window, renderer, "character/" + src)
{
	
}

void Character::draw(SDL_Renderer* renderer)
{
	pseudo.draw(renderer, getPseudoX(), getPseudoY());
	SDL_RenderCopy(renderer, text[animationID][dir][step], NULL, &this->pos);
}
