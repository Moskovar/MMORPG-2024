#pragma once
#include "UIElement.h"
#include <string>

class Portrait : public UIElement
{
public:
	Portrait(int x, int y, SDL_Renderer* renderer, SDL_Texture* textEntity);

	void setTextEntity(SDL_Texture* textEntity) { this->textEntity = textEntity; }
	bool hasTextEntity() { return !(textEntity == nullptr); }
	
	void draw(SDL_Renderer* renderer) override;

private:
	SDL_Surface* img  = nullptr;
	SDL_Texture* text = nullptr;

	SDL_Texture* textEntity = nullptr;
};

