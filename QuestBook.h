#pragma once
#include "UIElement.h"
#include <iostream>

#include <vector>
#include <SDL_image.h>
#include <string>

#include "Quest.h"

using namespace std;

#define MAXQUEST 20

class QuestBook : public UIElement
{
	public:
		QuestBook(){}
		QuestBook(int x, int y, SDL_Renderer* renderer);
		~QuestBook() { for (Quest* q : quest) { delete q; q = nullptr; } cout << "Quest book deleted !" << endl; }

		bool addQuest(Quest* q);
		void draw(SDL_Renderer* renderer) override;

	private:
		SDL_Surface* img = nullptr;
		SDL_Texture* text = nullptr;

		Quest* quest[MAXQUEST];
};

