#include "Element.h"

Element::Element(float xMap, float yMap, int width, int height)
{
	this->xOffset = 0;
	this->yOffset = 0;

	this->xMap = xMap;
	this->yMap = yMap;

	this->rowMap = xMap / 1900;
	this->colMap = yMap / 1080;

	this->x = (int)xMap % 1900;
	this->y = (int)yMap % 1080;

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
