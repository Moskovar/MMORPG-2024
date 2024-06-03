#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <Windows.h>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <chrono>

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
		bool isDmgDone() { return dmgDone; }
		short getDmg() { return this->dmg; }

		void setDmgDone() { last_dmg_time = std::chrono::high_resolution_clock::now(); dmgDone = true; }
		void checkDmgDone();

		map<float, SDL_Surface*[30]>& getImg()  { return img;  }
		map<float, SDL_Texture*[30]>& getText() { return text; }

	protected:
		string name = "";
		short range = 0, dmg = 0, step = 0;
		float boostSpeed = 1;
		bool moving = false;

		bool dmgDone = false;//faire une classe intermédiaire ? pour spell soutien / spell dmg ? -> problème si je veux un spell qui do both ? faire une troisième classe ?
		short hitSpeed = 0;//coups par seconde
		std::chrono::time_point<std::chrono::high_resolution_clock> last_dmg_time;

		map<float, SDL_Surface* [30]> img;
		map<float, SDL_Texture* [30]> text;
};