#include "Warrior.h"

Warrior::Warrior(std::string name, float xMap, float yMap, int id, int category, SDL_Renderer* renderer) : Character(name, xMap, yMap, id, category, "warrior", renderer)
{
	updateBoxes();
}
