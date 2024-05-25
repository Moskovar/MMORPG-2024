#pragma once
#include <chrono>
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

	struct MoveRate {//utile ??
		float xRate, yRate;
	};

	struct NetworkEntity {
		short id = 0, countDir = 0;
		int x = 0, y = 0;
		uint64_t timestamp; // En microsecondes depuis l'epoch
	};

	uint64_t getCurrentTimestamp();
	
	extern map<int, map<int, string>> categories;
	extern map<float, MoveRate> pixDir;//useless??

	Uint32 get_pixel(SDL_Surface* surface, int x, int y);

}
