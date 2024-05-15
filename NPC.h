#pragma once
#include "Entity.H"
class NPC : Entity
{
	public:
		NPC() {}
		NPC(std::string name, int category, SDL_Window* window, SDL_Renderer* renderer, string className);
	private:
};

