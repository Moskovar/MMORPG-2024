#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include "Element.h"

class MapFragment : public Element
{
	public:
		MapFragment() {}
		MapFragment(string src, SDL_Renderer* renderer);

		bool isVisible() { return visible; }

		void setVisible(bool state) { visible = state; }
		void setX(int x) { this->pos.x = x; }
		void setY(int y) { this->pos.y = y; }

		bool isInFront(int x, int y) { return false; }
		bool check_collisions(int x, int y) { return false; }
		void draw(SDL_Renderer* renderer) { SDL_RenderCopy(renderer, text, NULL, &pos); }

	private:
		bool visible = false;

		SDL_Surface* img  = nullptr;
		SDL_Texture* text = nullptr;
};

