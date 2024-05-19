#include "Map.h"

Map::Map(int r, int c) : rows(r), cols(c)
{
	fragments = new MapFragment*[rows];
	for (int i = 0; i < rows; i++) fragments[i] = new MapFragment[cols];
}

Map::~Map()
{
	for (int i = 0; i < rows; i++) { delete fragments[i]; fragments[i] = nullptr; }
	delete fragments;
	fragments = nullptr;

	printf("Map cleared !\n");
}

void Map::addOffset(float xOffset, float yOffset)
{
	for (int i = 0; i < rows; i++) 
		for (int j = 0; j < cols; j++) 
		{ 
			fragments[i][j].addXOffset(xOffset); 
			fragments[i][j].addYOffset(yOffset); 
		}
}

void Map::draw(SDL_Renderer* renderer)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (fragments[i][j].isVisible()) fragments[i][j].draw(renderer);
}
