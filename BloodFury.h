#pragma once
#include "Spell.h"
#include "Entity.h"

class BloodFury :
    public Spell
{
    public:
		BloodFury(SDL_Renderer* renderer, bool drawUI);
		void run(Entity& player) override;
		void runOthers(Entity& player) override;
		void resetSpell(Entity& player) override;
		bool isInRange(uti::Circle player, uti::Circle enemy) override;
		void update(Entity& p) override;

    private:

};

