#pragma once
#include "Element.h"
class Building : public Element
{
public:
	Building() {}
	Building(float x, float y, int width, int height, SDL_Renderer* renderer, string src);
	~Building();

	bool isInFront(int x, int y) override;
	bool check_collisions(int x, int y) override;

	virtual void draw(SDL_Renderer* renderer);
	void resetPos() override;
private:
	SDL_Surface* img   = nullptr;
	SDL_Surface* imgBg = nullptr;
	SDL_Texture* text  = nullptr;

};

