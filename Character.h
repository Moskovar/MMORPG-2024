#pragma once
#include "Entity.h"

class Character : public Entity
{
	public:
		Character() {}
		Character(std::string name, float x, float y, int category, string src, SDL_Renderer* renderer);

		void resetPos() override;
		void draw(SDL_Renderer* renderer);
};

