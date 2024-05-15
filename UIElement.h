#pragma once
#include <SDL.h>

using namespace std;

class UIElement
{
	public:
		UIElement() {}
		UIElement(int x, int y, int width, int height);

		virtual void draw(SDL_Renderer* renderer) = 0;

		bool isToDraw() { return toDraw; }

		void setToDraw(bool state) { toDraw = state; }

	protected:
		SDL_Rect pos = { 0, 0, 0, 0 };
		bool toDraw = false;
};

