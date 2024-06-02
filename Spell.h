#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <cmath>

#include "Element.h"
#include "Map.h"
#include "uti.h"
#include <mutex>

class Entity;

using namespace std;

class Spell
{

	public:
		Spell(string name);
		virtual void run(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy) = 0;
		virtual void runOthers(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy) = 0;
		virtual void resetSpell(Entity& player) = 0;
		virtual bool isInRange(uti::Circle player, uti::Circle enemy) = 0;
		short getRange() { return this->range; }

		float getBoostSpeed() { return this->boostSpeed; }
		bool isMoving() { return moving; }
		map<float, SDL_Surface*[30]>& getImg()  { return img;  }
		map<float, SDL_Texture*[30]>& getText() { return text; }

	protected:
		string name = "";
		short range = 0, dmg = 0, step = 0;
		float boostSpeed = 1;
		bool moving = false;

		map<float, SDL_Surface* [30]> img;
		map<float, SDL_Texture* [30]> text;
};