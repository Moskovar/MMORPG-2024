#pragma once
#include "Spell.h"
#include "Entity.h"

class AutoAttack : public Spell
{
	public:
		AutoAttack(SDL_Renderer* renderer);
		void run(Entity& player) override;
		virtual void runOthers (Entity& player) override;
		virtual void resetSpell(Entity& player) override;
		bool isInRange(uti::Circle player, uti::Circle enemy) override;

		static const short animationID = 3;
};

