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
	virtual void runOthers (Entity& player) override;
	virtual void resetSpell(Entity& player) override;
	bool isInRange(uti::Circle player, uti::Circle enemy) override;

	static const short animationID = 4;

private:
	

};