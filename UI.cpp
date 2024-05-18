#include "UI.h"

SDL_Surface* UI::imgQuest_available = nullptr;
SDL_Texture* UI::textQuest_available = nullptr;

UI::UI(SDL_Renderer* renderer, Entity* e)
{
	imgQuest_available = IMG_Load("img/ui/quest/quest_icon_available.png");
	if (imgQuest_available) textQuest_available = SDL_CreateTextureFromSurface(renderer, imgQuest_available);
	else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: quest_icon_available.png");   exit(0); }

	qb = new QuestBook(850, 200, renderer);
	targetPortrait = new Portrait(350, 0, renderer, nullptr);
	v_UIElements.push_back(new Portrait(0   , 0  , renderer, e->getPortraitTexture()));
	v_UIElements.push_back(targetPortrait);
	v_UIElements.push_back(qb);
}

UI::~UI()
{
	SDL_FreeSurface(imgQuest_available);
	SDL_DestroyTexture(textQuest_available);

	for (UIElement* e : v_UIElements)//check si tout est bien clear (car pas dans vecteur)
	{
		delete e;
		e = nullptr;
	}
		printf("UI cleared !\n");
}

void UI::draw(SDL_Renderer* renderer)
{
	for (UIElement* e : v_UIElements) e->draw(renderer);
}
