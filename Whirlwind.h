#pragma once
#include "Spell.h"
#include <iostream>

#include "Entity.h"

using namespace std;

class Whirlwind : public Spell
{
public:
	Whirlwind(SDL_Renderer* renderer);
	void run(Entity& e) override;
	void runOthers (Entity& player) override;
	void resetSpell(Entity& player) override;
	bool isInRange(uti::Circle player, uti::Circle enemy) override;
	void update(Entity& p) override;

private:
	

};