#include "Element.h"

Element::Element(float x, float y, int width, int height)
{
	this->xOffset = 0;
	this->yOffset = 0;

	this->x = x;
	this->y = y;

	this->pos.x = x;	this->pos.y = y;	this->pos.w = width;	this->pos.h = height;
}

void Element::resetPos()
{
	x += xOffset;
	pos.x = x;
	y += yOffset;
	pos.y = y;
	xOffset = 0;
	yOffset = 0;
}
