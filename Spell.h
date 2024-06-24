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
#include "SpellIcon.h"

class Entity;

using namespace std;

class Entity;

class Spell
{

	public:
		Spell(string name);
		virtual void run(Entity& player) = 0;
		virtual void runOthers(Entity& player) = 0;
		virtual void resetSpell(Entity& player) = 0;
		virtual bool isInRange(uti::Circle player, uti::Circle enemy) = 0;
		virtual void update(Entity& p);
		short getRange() { return this->range; }
		short getID() { return this->id; }
		short getHitSpeed() { return hitSpeed; }
		string getName() { return this->name; }

		float getBoostSpeed() { return this->boostSpeed; }
		bool isMoving() { return moving; }
		bool isDmgDone() { return dmgDone; }
		short getDmg() { return this->dmg; }
		bool isAvailable() { return chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - last_time_used).count() >= cd; }
		bool isCancelable() { return cancelable;  }
		bool isAoe() { return aoe; }

		map<float, SDL_Surface*[30]>& getImg()  { return img;  }
		map<float, SDL_Texture*[30]>& getText() { return text; }

		void resetStep() { step = 0; }
		void start_cd() { last_time_used = chrono::high_resolution_clock::now(); }

		void draw(SDL_Renderer* renderer);

	protected:
		string name = "";
		short range = 0, dmg = 0, step = 0, id = 0;
		float boostSpeed = 1;
		bool moving = false, cancelable = true, aoe = false;

		bool dmgDone = false;//faire une classe intermédiaire ? pour spell soutien / spell dmg ? -> problème si je veux un spell qui do both ? faire une troisième classe ?
		short hitSpeed = 0;//coups par seconde

		map<float, SDL_Surface* [30]> img;
		map<float, SDL_Texture* [30]> text;

		SpellIcon spell_icon;
		chrono::high_resolution_clock::time_point last_time_used;
		int cd = 0;
};