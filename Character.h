#pragma once
#include "Entity.h"

class Character : public Entity
{
	public:
		Character() {}
		Character(std::string name, float x, float y, int category, SDL_Window* window, SDL_Renderer* renderer, string src);

		void resetPos() override;
		void draw(SDL_Renderer* renderer);
};

