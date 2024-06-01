#pragma once
#include "Spell.h"
#include "Entity.h"

class AutoAttack : public Spell
{
	public:
		AutoAttack(SDL_Renderer* renderer);
		void run(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& e, Entity* enemy) override;
		virtual void runOthers(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy) override;
		virtual void resetSpell(Entity& player) override;

		static const short id = 3, animationID = 3;
};

