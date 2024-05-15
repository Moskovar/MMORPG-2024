#include "QuestBook.h"

QuestBook::QuestBook(int x, int y, SDL_Renderer* renderer) : UIElement(x, y, 916, 667)
{
	img = IMG_Load("img/ui/questbook.png");

	if (img) text = SDL_CreateTextureFromSurface(renderer, img);
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: img/ui/questbook.png"); exit(0); }
}

void QuestBook::draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, text, NULL, &pos);
}
