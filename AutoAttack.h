#pragma once
#include "Spell.h"
#include "Entity.h"

class AutoAttack : public Spell
{
	public:
		AutoAttack(SDL_Renderer* renderer);
		void run(vector<Element*>& v_elements, Entity& e, vector<MapFragment*>& v_mapFragments, bool& cameraLock) override;

		static const short animationID = 3;
};

