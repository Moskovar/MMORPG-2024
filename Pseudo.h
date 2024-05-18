#pragma once
#include <SDL.h>

#include <string>

#include "Font.h"

class Pseudo
{
	public:
		Pseudo(){}
		Pseudo(string name, SDL_Renderer* renderer);

		void clear();

		int getWidth() { return text.getWidth(); }
		Font& getFont() { return this->text; }
		SDL_Texture* getTexture() { return text.getTexture(); }

		void draw(SDL_Renderer* renderer, int x, int y);

	private:
		Font text;
		SDL_Rect pos;
};

