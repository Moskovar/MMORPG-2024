#pragma once
#include <SDL.h>

#include <string>


using namespace std;

class Job
{
public:
	Job() {}
	Job(string name, short lvl);
	string getName() { return this->name; }
private:
	string name = "";
	short  lvl  = 0;

	SDL_Surface* img  = nullptr;
	SDL_Texture* text = nullptr;
};

