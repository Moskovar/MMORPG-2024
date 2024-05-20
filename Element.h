#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <cstdlib>

using namespace std;

class Element
{
public:
	Element(){}
	Element(float x, float y, int width, int height);

	virtual void draw(SDL_Renderer* renderer)   = 0;
	virtual bool isInFront(int x, int y)        = 0;
	virtual bool check_collisions(int x, int y) = 0;
	virtual void resetPos();
	virtual void addXOffset(float xOffset) { this->xOffset += xOffset; }
	virtual void addYOffset(float yOffset) { this->yOffset += yOffset; }
	virtual void updateXOffset(float cameraSpeed) { x += cameraSpeed; pos.x = x; xOffset -= cameraSpeed; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
	virtual void updateYOffset(float cameraSpeed) { y += cameraSpeed; pos.y = y; yOffset -= cameraSpeed; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite

	SDL_Rect getPos()  { return pos;	 }
	float getX()	   { return x;		 }
	float getY()	   { return y;		 }
	float getXOffset() { return xOffset; }
	float getYOffset() { return yOffset; }
	int   getXMap()    { return xMap;    }
	int   getYMap()    { return yMap;    }

	void setXYMap(int x, int y) { this->xMap = x;	this->yMap = y; }
	void setX(float x) { this->x = x; pos.x = x; }
	void setY(float y) { this->y = y; pos.y = y; }
	void addXMap(int x) { xMap += x; }
	void addYMap(int y) { yMap += y; }
	void addX(float x) { this->x += x; pos.x = this->x; }
	void addY(float y) { this->y += y; pos.y = this->y; }
	void resetPos(float xOffset, float yOffset) { addXOffset(x); addYOffset(y); resetPos(); }
	void updateMapPos(int x, int y) { this->xMap += x;	this->yMap += y; }


protected:
	SDL_Rect pos{ 0, 0, 0, 0};
	bool isSolid = false;
	int xMap = 0, yMap = 0;//position sur la map
	float x = 0, y = 0, xOffset = 0, yOffset = 0;//position sur l'écran et offset par rapport à la caméra

};

