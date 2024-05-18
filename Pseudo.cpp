#include "Pseudo.h"
#include <iostream>

Pseudo::Pseudo(string name, SDL_Renderer* renderer)
{
	text = Font(name, 18, 255, 255, 255, 255, renderer);
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
