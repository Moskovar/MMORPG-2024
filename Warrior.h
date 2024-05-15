#pragma once
#include "Character.h"
class Warrior : public Character
{
	public:
		Warrior() : Character() {}
		Warrior(std::string name, int category, SDL_Window* window, SDL_Renderer* renderer);
	private:

};

