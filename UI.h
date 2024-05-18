#pragma once
#include "UIElement.h"

#include <vector>

#include "Entity.h"
#include "QuestBook.h"
#include "Portrait.h"
#include "Quest.h"
#include "ScreenMsg.h"

class UI
{
	public:
		UI() {}
		UI(SDL_Window* window, SDL_Renderer* renderer, Entity* e);
		~UI();

		static SDL_Surface* imgQuest_available;
		static SDL_Texture* textQuest_available;

		bool isQBVisible() { return qb->isVisible(); }
		bool isScreenMsgVisible() { return screenMsg->isVisible(); }
		QuestBook* getQuestBook() { return this->qb; }

		void setTargetPortrait(SDL_Texture* textTarget) { targetPortrait->setTextEntity(textTarget); }
		void setQBVisible(bool state) { qb->setVisible(state); }
		void setScreenMsgVisible(bool state) { screenMsg->setVisible(state); }
		void addUIElement(UIElement* e) { v_UIElements.push_back(e); }
		void draw(SDL_Renderer* renderer);

	private:
		QuestBook* qb = nullptr;
		Portrait* targetPortrait = nullptr;
		ScreenMsg* screenMsg = nullptr;
		vector<UIElement*> v_UIElements;


};

