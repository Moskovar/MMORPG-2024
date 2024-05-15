#pragma once
#include "Spell.h"
#include <iostream>

#include "Entity.h"

using namespace std;

class Whirlwind : public Spell
{
public:
	Whirlwind(SDL_Renderer* renderer);
	void run(vector<Element*>& v_element, Entity& e, bool& camereLock) override;

	static const short animationID = 4;

private:


};