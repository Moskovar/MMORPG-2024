#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <Windows.h>
#include <string>
#include <map>
#include <vector>

#include "uti.h"

#include "Element.h"

class Entity;

using namespace std;

class Spell
{

	public:
		//Spell() {};
		Spell(string name);
		virtual void run(vector<Element*>& v_elements, Entity& e, bool& cameraLock) = 0;

		map<float, SDL_Surface*[30]>& getImg()  { return img;  }
		map<float, SDL_Texture*[30]>& getText() { return text; }

		//bool isActive() { return active; }

		//void setActive(bool active) { this->active = active; }

	protected:
		string name;
		short step;
		//bool active;

		map<float, SDL_Surface* [30]> img;
		map<float, SDL_Texture* [30]> text;
};