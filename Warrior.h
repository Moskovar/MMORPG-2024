#pragma once
#include "Character.h"
class Warrior : public Character
{
	public:
		Warrior() : Character() {}
		Warrior(std::string name, float xMap, float yMap, int id, int category, SDL_Renderer* renderer);
	private:

};

