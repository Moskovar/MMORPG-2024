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

#pragma pack(push, 1)
	struct NetworkEntity {
		short header = 0;
		short id    = 0, countDir = 0, hp = 0;
		int   xMap  = 0, yMap     = 0;
		uint64_t timestamp; // En microsecondes depuis l'epoch
	};
#pragma pack(pop)

#pragma pack(push, 1)
	struct NetworkEntitySpell {
		short header = 1;
		short id = 0, spellID = 0;
	};
#pragma pack(pop)

	uint64_t getCurrentTimestamp();
	
	extern map<int, map<int, string>> categories;
	extern map<float, MoveRate> pixDir;//useless??

	Uint32 get_pixel(SDL_Surface* surface, int x, int y);

}
