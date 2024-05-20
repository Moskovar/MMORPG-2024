#pragma once
#include "Spell.h"
#include <iostream>

#include "Entity.h"

using namespace std;

class Whirlwind : public Spell
{
public:
	Whirlwind(SDL_Renderer* renderer);
	void run(vector<Element*>& v_element, vector<Element*> v_elements_solid, Entity& e, Entity* enemy, Map* m, bool& camereLock, mutex* mtx) override;

	static const short animationID = 4;

private:


};