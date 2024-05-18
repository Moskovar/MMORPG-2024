#pragma once
#include "UIElement.h"
#include "Font.h"

class ScreenMsg : public UIElement
{
	public:
		ScreenMsg(string text, int policeSize, SDL_Renderer* renderer);

		void setPos(int x, int y) { pos.x = x; pos.y = y; }
		void updatePos() { msg.updatePos(); }
		void draw(SDL_Renderer* renderer) { if(visible) msg.draw(renderer); }

	private:
		Font msg;
};

