#include "SpellIcon.h"
#include "Spell.h"

SpellIcon::SpellIcon(string characterClass, Spell* spell, SDL_Renderer* renderer)
{
	img = IMG_Load(string("img/entity/character/" + characterClass +  "/spell_icons/" + spell->getName() + ".png").c_str());
	if (img) text = SDL_CreateTextureFromSurface(renderer, img);
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, string("Failed to load image: spell icon -> " + characterClass + "/" + spell->getName()).c_str()); exit(0); }

	pos.x = 500;
	pos.y = 900;
	pos.w = img->w;
	pos.h = img->h;
}

void SpellIcon::draw(SDL_Renderer* renderer)
{
	if(text) SDL_RenderCopy(renderer, text, NULL, &pos);
}
