#include "QuestBook.h"

QuestBook::QuestBook(int x, int y, SDL_Renderer* renderer) : UIElement(x, y, 916, 667)
{
	img = IMG_Load("img/ui/questbook.png");

	if (img) text = SDL_CreateTextureFromSurface(renderer, img);
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: img/ui/questbook.png"); exit(0); }
}

bool QuestBook::addQuest(Quest* q)
{
	for (int i = 0; i < MAXQUEST; i++) 
		if (this->quest[i] == nullptr) 
		{
			quest[i] = q;
			return true;
		}
	return false;
}

void QuestBook::draw(SDL_Renderer* renderer)
{
	if (visible)
	{
		SDL_RenderCopy(renderer, text, NULL, &pos);
		for (Quest* q : quest) if (q != nullptr) { q->drawName(renderer, this->pos.x + this->pos.w / 2 + 50, this->pos.y + 100); q->drawDesc(renderer, this->pos.x + this->pos.w / 2 + 150, this->pos.y + 100); }
	}
}
