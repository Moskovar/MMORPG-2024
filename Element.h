#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;

class Element
{
public:
	Element(){}
	Element(float x, float y, int width, int height);

	float& getXOffset() { return xOffset; }
	float& getYOffset() { return yOffset; }

	void addXOffset(float xOffset) { this->xOffset += xOffset; }
	void addYOffset(float yOffset) { this->yOffset += yOffset; }
	void updateXOffset(float& cameraSpeed) { x += cameraSpeed; pos.x = x; xOffset -= cameraSpeed; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
	void updateYOffset(float& cameraSpeed) { y += cameraSpeed; pos.y = y; yOffset -= cameraSpeed; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
	void resetPos() { x += xOffset; pos.x = x; y += yOffset; pos.y = y; xOffset = 0; yOffset = 0; }
	virtual void draw(SDL_Renderer* renderer) = 0;

protected:
	SDL_Rect pos{ 0, 0, 0, 0};
	int xMap = 0, yMap = 0;//position sur la map
	float x = 0, y = 0, xOffset = 0, yOffset = 0;//position sur l'écran et offset par rapport à la caméra

};

