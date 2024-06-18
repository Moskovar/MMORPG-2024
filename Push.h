#pragma once
#include "Spell.h"
#include "Entity.h"

class Push : public Spell
{
	public:
		Push(SDL_Renderer* renderer);
		void run(Entity& player) override;
		void runOthers(Entity& player) override;
		void resetSpell(Entity& player) override;
		bool isInRange(uti::Circle player, uti::Circle enemy) override;
		void update(Entity& player) override;

		short getMoveX() { return this->moveX; }
		short getMoveY() { return this->moveY; }

	private:
		float moveX = 0.0f, moveY = 0.0f;
};

