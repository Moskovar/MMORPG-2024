#pragma once
#include <SDL.h>
#include <string>

#include "Font.h"

class Quest
{
	public:
		Quest() {}
		Quest(string name, string desc, int exp, SDL_Window* window, SDL_Renderer* renderer);

		string getName() { return this->name.getText(); }
		string getDesc() { return this->desc.getText(); }
		int    getExp()  { return this->exp;            }

		void drawName(SDL_Renderer* renderer, int x, int y);
		void drawDesc(SDL_Renderer* renderer, int x, int y);

	private:
		Font name, desc;
		int exp = 0;
		//Item items[5];
};

