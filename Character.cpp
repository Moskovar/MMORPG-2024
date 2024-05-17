#include "Character.h"

Character::Character(std::string name, float x, float y, int category, SDL_Window* window, SDL_Renderer* renderer, string src) : Entity(name, x, y, category, window, renderer, "character/" + src)
{
	clickBox.x = x + 90;
	clickBox.y = y + 65;
	clickBox.w = 75;
	clickBox.h = 125;
};

void Character::draw(SDL_Renderer* renderer)
{
	//pseudo.draw(renderer, getPseudoX(), getPseudoY());
	pseudo.draw(renderer, getPseudoX(), getPseudoY());
	SDL_RenderCopy(renderer, text[animationID][dir][step], NULL, &this->pos);
}
