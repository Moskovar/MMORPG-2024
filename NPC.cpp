#include "NPC.h"
#include "UI.h"

NPC::NPC(std::string name, float x, float y, int id, int category, string className, bool isSolid, SDL_Renderer* renderer) : Entity(name, x, y, id, category, className, renderer)
{
	clickBox.x = x + 90;
	clickBox.y = y + 65;
	clickBox.w = 75;
	clickBox.h = 125;

	quest = Quest("First quest", "First quest in the world", 100, renderer);
	quest.iconPos.x = this->x + 125 - 17 / 2;
	quest.iconPos.y = this->y - 25;
}

void NPC::resetPos()
{
	x += xOffset;
	pos.x = x;
	y += yOffset;
	pos.y = y;
	xOffset = 0;
	yOffset = 0;

	updateMovebox();
	updateClickBox();

	quest.iconPos.x = this->x + 125 - 17 / 2;
	quest.iconPos.y = this->y - 25;
}

void NPC::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, text[animationID][dir][step], NULL, &this->pos);
	if (quest.getState() == 1) SDL_RenderCopy(renderer, UI::textQuest_available, NULL, &quest.iconPos);
	pseudo.draw(renderer, getPseudoX(), getPseudoY());

}
