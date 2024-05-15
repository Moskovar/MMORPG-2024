#pragma once
#include "UIElement.h"

#include <vector>
#include <SDL_image.h>
#include <string>

class QuestBook : public UIElement
{
	public:
		QuestBook(){}
		QuestBook(int x, int y, SDL_Renderer* renderer);

		void draw(SDL_Renderer* renderer) override;

	private:
		SDL_Surface* img = nullptr;
		SDL_Texture* text = nullptr;
};

