#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <map>
#include <string>

using namespace std;


namespace uti {
	enum Language {
		FR,
		ENG
	};

	enum Category {
		PLAYER,
		NPC
	};

	enum Direction {
		UP	  = 1,
		RIGHT = 3,
		DOWN  = 6,
		LEFT  = 11
	};

	struct MoveRate {
		float xRate, yRate;
	};
	
	extern map<int, map<int, string>> categories;
	extern map<float, MoveRate> pixDir;

	Uint32 get_pixel(SDL_Surface* surface, int x, int y);

}
