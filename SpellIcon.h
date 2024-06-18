#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include "Font.h"

class Spell;

class SpellIcon
{
	public:
		SpellIcon() {};
		SpellIcon(string characterClass, Spell* spell, SDL_Renderer* renderer);

		void draw(SDL_Renderer* renderer);

	private:
		SDL_Rect pos{ 0, 0, 0, 0 };

		Font shortcut;
		Font timer;

		SDL_Surface* img  = nullptr;
		SDL_Texture* text = nullptr;
};

