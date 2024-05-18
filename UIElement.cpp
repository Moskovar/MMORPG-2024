#include "UIElement.h"

UIElement::UIElement(int x, int y, int width, int height)
{
	this->pos.x = x;
	this->pos.y = y;
	this->pos.w = width;
	this->pos.h = height;

	//imgQuest_available = IMG_Load("img/ui/quest/quest_icon_available.png");
}

UIElement::~UIElement()
{

}
