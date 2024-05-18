#pragma once
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class UIElement
{
	public:
		UIElement() {}
		UIElement(int x, int y, int width, int height);
		~UIElement();

		virtual void draw(SDL_Renderer* renderer) = 0;

		bool isVisible() { return visible; }

		void setVisible(bool state) { visible = state; }

	protected:
		SDL_Rect pos = { 0, 0, 0, 0 };
		bool visible = false;		
};

