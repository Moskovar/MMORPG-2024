#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>

#include "Font.h"

class Spell;

class SpellIcon
{
	public:
		SpellIcon() {};
		//~SpellIcon();//A FAIRE!!!
		SpellIcon(string characterClass, Spell* spell, short posIndex, SDL_Renderer* renderer);

		short getTimer() { return timer; }

		void setCDIcon(bool state) { SDL_SetTextureAlphaMod(text, (state) ? 100 : 255); }
		void setTimer(short cd) { timer = cd; }

		void draw(SDL_Renderer* renderer);

	private:
		SDL_Rect pos{ 0, 0, 50, 50 };

		Font shortcut;
		map<short, Font> timers;
		short timer = 0;

		SDL_Surface* img  = nullptr;
		SDL_Texture* text = nullptr;

		SDL_Renderer* renderer;
};

