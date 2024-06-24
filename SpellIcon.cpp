#include "SpellIcon.h"
#include "Spell.h"

SpellIcon::SpellIcon(string characterClass, Spell* spell, short posIndex, SDL_Renderer* renderer)
{
	this->renderer = renderer;

	img = IMG_Load(string("img/entity/character/" + characterClass +  "/spell_icons/" + spell->getName() + ".png").c_str());
	if (img) text = SDL_CreateTextureFromSurface(renderer, img);
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, string("Failed to load image: spell icon -> " + characterClass + "/" + spell->getName()).c_str()); exit(0); }

	// Définir le mode de blending pour utiliser l'alpha
	SDL_SetTextureBlendMode(text, SDL_BLENDMODE_BLEND);

	pos.x = 600 + posIndex * 65;
	pos.y = 900;
	pos.w = 50;
	pos.h = 50;

	for (short i = 0; i < 60; i++)
	{
		timers[i] = Font(to_string(i), 18, 255, 255, 255, 255, renderer);
		SDL_Rect pos = { 0, 0, timers[i].getWidth(), timers[i].getHeight() };

		SDL_QueryTexture(timers[i].getTexture(), NULL, NULL, &pos.x, &pos.y); // Taille du texte

		timers[i].setPos(this->pos.x + 15, this->pos.y + 15);
	}

	SDL_SetTextureAlphaMod(text, 255);
}

void SpellIcon::draw(SDL_Renderer* renderer)
{
	if (!text) return;

	SDL_RenderCopy(renderer, text, NULL, &pos);
	if(timer != 0) timers[timer].draw(renderer);
}
