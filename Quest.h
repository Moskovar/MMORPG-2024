#pragma once
#include <SDL.h>
#include <string>

#include "Font.h"

class Quest
{
	public:
		Quest() {}
		Quest(string name, string desc, int exp, SDL_Window* window, SDL_Renderer* renderer);

		SDL_Rect iconPos;

		short getState() { return state; }
		string getName() { return this->name.getText(); }
		string getDesc() { return this->desc.getText(); }
		int    getExp()  { return this->exp;            }

		void drawName(SDL_Renderer* renderer, int x, int y);
		void drawDesc(SDL_Renderer* renderer, int x, int y);

	private:
		short state = 1;//0 pas dispo, 1 dispo, 2 attente d'être retournée
		Font name, desc;
		int exp = 0;
		//Item items[5];
};

