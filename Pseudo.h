#pragma once
#include <SDL.h>

#include <string>

#include "Font.h"

class Pseudo
{
	public:
		Pseudo(){}
		Pseudo(SDL_Window* window, SDL_Renderer* renderer, string name);

		void clear();

		int getWidth() { return text.getWidth(); }
		Font& getFont() { return this->text; }
		SDL_Texture* getTexture() { return text.getTexture(); }

		void updatePos(int x, int y) { this->pos.x = x; this->pos.y = y; }
		void draw(SDL_Renderer* renderer, int x, int y);

	private:
		Font text;
		SDL_Rect pos;
};

