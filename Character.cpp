#include "Character.h"

Character::Character(std::string name, float x, float y, int id, int category, string src, SDL_Renderer* renderer) : Entity(name, x, y, id, category, "character/" + src, renderer)
{
	clickBox.x = x + 90;
	clickBox.y = y + 65;
	clickBox.w = 75;
	clickBox.h = 125;
};

void Character::resetPos()
{
    x += xOffset;
    pos.x = x;
    y += yOffset;
    pos.y = y;
    xOffset = 0;
    yOffset = 0;

    updateMovebox();
    updateClickBox();
}

void Character::draw(SDL_Renderer* renderer)
{
	//pseudo.draw(renderer, getPseudoX(), getPseudoY());
	pseudo.draw(renderer, getPseudoX(), getPseudoY());
	SDL_RenderCopy(renderer, text[animationID][dir][step / ANIMATIONMULTIPL], NULL, &this->pos);
}
