#pragma once
#include "Entity.h"
#include "Quest.h"

class NPC : public Entity
{
	public:
		NPC() {}
		NPC(std::string name, float x, float y, int category, SDL_Window* window, SDL_Renderer* renderer, string className, bool isSolid);


		void resetPos() override;
		void draw(SDL_Renderer* renderer);
	private:
		bool hasQuest;
		Quest quest;

};

