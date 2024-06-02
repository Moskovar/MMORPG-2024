#pragma once
#include "Spell.h"
#include <iostream>

#include "Entity.h"

using namespace std;

class Whirlwind : public Spell
{
public:
	Whirlwind(SDL_Renderer* renderer);
	void run(vector<Element*>& v_element, vector<Element*> v_elements_solid, Entity& e, Entity* enemy) override;
	virtual void runOthers(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy) override;
	virtual void resetSpell(Entity& player) override;
	bool isInRange(uti::Circle player, uti::Circle enemy) override;

	static const short id = 4, animationID = 4;

private:
	

};