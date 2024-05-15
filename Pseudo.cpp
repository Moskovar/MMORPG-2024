#include "Pseudo.h"

Pseudo::Pseudo(SDL_Window* window, SDL_Renderer* renderer, string name)
{
	text = Font(window, renderer, name);
	pos = { 0, 0, 0, 0 };
}

void Pseudo::clear()
{
	text.clear();
}

void Pseudo::draw(SDL_Renderer* renderer, int x, int y)
{
	updatePos(x, y);
	//SDL_QueryTexture(text.getTexture(), NULL, NULL, &pos.x, &pos.y); // Taille du texte
	//SDL_RenderCopy(renderer, text.getTexture(), NULL, &pos);
}
