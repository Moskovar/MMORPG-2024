#pragma once
#include "Spell.h"
#include "Entity.h"

class AutoAttack : public Spell
{
	public:
		AutoAttack(SDL_Renderer* renderer);
		void run(Entity& player) override;
		void runOthers (Entity& player) override;
		void resetSpell(Entity& player) override;
		bool isInRange(uti::Circle player, uti::Circle enemy) override;
};

