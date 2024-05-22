#pragma once
#include "Character.h"
class Warrior : public Character
{
	public:
		Warrior() : Character() {}
		Warrior(std::string name, float x, float y, int id, int category, SDL_Renderer* renderer);
	private:

};

