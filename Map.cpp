#include "Map.h"
#include "Building.h"

Map::Map(int r, int c, SDL_Renderer* renderer) : rows(r), cols(c)
{
	fragments = new MapFragment**[rows];
	for (int i = 0; i < rows; i++)
	{
		fragments[i] = new MapFragment*[cols];
		for (int j = 0; j < cols; j++)
		{
			fragments[i][j] = nullptr;
		}
	}
}

Map::~Map()
{
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++)
		{
			delete fragments[i][j];
			fragments[i][j] = nullptr;
		}
		delete fragments[i]; 
		fragments[i] = nullptr; 
	}
	delete fragments;
	fragments = nullptr;

	printf("Map is cleared now !\n");
}

vector<Element*> Map::getElements(bool withFragments)
{
	vector<Element*> v_elements;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if(withFragments) v_elements.push_back(fragments[i][j]);
			for (Element* e : fragments[i][j]->getElements())
				v_elements.push_back(e);
		}
	return v_elements;
}

void Map::addFragment(int r, int c, string src, vector<Element*> v_elements, SDL_Renderer* renderer)
{
	fragments[r][c] = new MapFragment(src, renderer, v_elements);
}

void Map::addOffset(int r, int c, float xOffset, float yOffset)
{
	if (fragments[r][c])
	{
		fragments[r][c]->addXOffset(xOffset);
		fragments[r][c]->addYOffset(yOffset);
	}
}

void Map::addOffset(float xOffset, float yOffset)
{
	for (int i = 0; i < rows; i++) 
		for (int j = 0; j < cols; j++) 
		{ 
			fragments[i][j]->addXOffset(xOffset); 
			fragments[i][j]->addYOffset(yOffset); 
		}
}

void Map::draw(SDL_Renderer* renderer)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			fragments[i][j]->draw(renderer);
}
