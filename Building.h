#pragma once
#include "Element.h"
#include "Entity.h"
class Building : public Element
{
public:
	Building() {}
	Building(float x, float y, int width, int height, SDL_Renderer* renderer, string src);
	~Building();

	bool isMoveBoxInArea(int mbX, int mbY);//mb movebox
	bool isInFront(int x, int y) override;
	bool check_collisions(int x, int y) override;
	void getEntitiesInArea(const map<int, Entity*>& entities);
	bool isAllEntitiesDrawn() { for (Entity* e : entities) if (!e->isDrawn) return false; return true; }
	int debug() {
		int c = 0;
		for (Entity* e : entities) 
		{ 
			if (!e->isDrawn) c++;
		}
		return c;
	}

	virtual void draw(SDL_Renderer* renderer);
	vector<Entity*> entities;//vector of entities in the building area (drawing purpose)
	//void resetPos() override;
private:
	SDL_Surface* img   = nullptr;
	SDL_Surface* imgBg = nullptr;
	SDL_Texture* text  = nullptr;

};

