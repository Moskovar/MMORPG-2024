#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include "Element.h"

class MapFragment : public Element
{
	public:
		MapFragment(string src, SDL_Renderer* renderer);

		void setX(int x) { this->pos.x = x; }
		void setY(int y) { this->pos.y = y; }
		void updateXOffset(float& cameraSpeed) { x += cameraSpeed; pos.x = x; xOffset -= cameraSpeed; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
		void updateYOffset(float& cameraSpeed) { y += cameraSpeed; pos.y = y; yOffset -= cameraSpeed; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite


		bool isInFront(int x, int y) { return false; }
		bool check_collisions(int x, int y) { return false; }
		void draw(SDL_Renderer* renderer) { SDL_RenderCopy(renderer, text, NULL, &pos); }

	private:
		SDL_Surface* img  = nullptr;
		SDL_Texture* text = nullptr;
};

