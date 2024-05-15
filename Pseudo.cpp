#include "Pseudo.h"
#include <iostream>

Pseudo::Pseudo(SDL_Window* window, SDL_Renderer* renderer, string name)
{
	text = Font(name, 18, window, renderer);
	pos = { 0, 0, text.getWidth(), text.getHeight()};

	SDL_QueryTexture(text.getTexture(), NULL, NULL, &pos.x, &pos.y); // Taille du texte
}

void Pseudo::clear()
{
	text.clear();
}

void Pseudo::draw(SDL_Renderer* renderer, int x, int y)
{
	text.draw(renderer, x, y);
}
