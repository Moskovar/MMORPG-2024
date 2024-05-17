#pragma once
#include "Entity.H"
class NPC : public Entity
{
	public:
		NPC() {}
		NPC(std::string name, float x, float y, int category, SDL_Window* window, SDL_Renderer* renderer, string className);

		void draw(SDL_Renderer* renderer);
	private:
};

