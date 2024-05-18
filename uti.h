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
	
	inline bool loadTexture(SDL_Texture* text, SDL_Surface* img, SDL_Renderer* renderer, string errMsg)
	{
		if (img) { text = SDL_CreateTextureFromSurface(renderer, img); }
		else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + errMsg).c_str()); exit(0); }
	}

	extern map<int, map<int, string>> categories;
	extern map<float, MoveRate> pixDir;
}
