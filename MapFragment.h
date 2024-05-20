#pragma once
#include "Element.h"

#include <SDL.h>
#include <SDL_image.h>

#include <vector>

class MapFragment : public Element
{
	public:
		MapFragment() {}
		MapFragment(string src, SDL_Renderer* renderer, vector<Element*> v_elements);
		~MapFragment();

		bool isVisible() { return visible; }
		bool isInFront(int x, int y) { return false; }
		bool check_collisions(int x, int y);
		vector<Element*> getElements() { return this->v_elements; }


		void draw(SDL_Renderer* renderer) { if (visible) SDL_RenderCopy(renderer, text, NULL, &pos); }
		void setVisible(bool state) { visible = state; }
		void setX(int x) { this->pos.x = x; }
		void setY(int y) { this->pos.y = y; }

		void addXOffset(float xOffset)		  override { this->xOffset += xOffset;	for (Element* e : v_elements) e->addXOffset(xOffset); }
		void addYOffset(float yOffset)		  override { this->yOffset += yOffset;	for (Element* e : v_elements) e->addYOffset(yOffset); }
		void updateXOffset(float cameraSpeed) override { x += cameraSpeed; pos.x = x; xOffset -= cameraSpeed; for (Element* e : v_elements) e->updateXOffset(cameraSpeed); }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
		void updateYOffset(float cameraSpeed) override { y += cameraSpeed; pos.y = y; yOffset -= cameraSpeed; for (Element* e : v_elements) e->updateYOffset(cameraSpeed); }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
		void resetPos() override;

	private:
		bool visible = true;
		vector<Element*> v_elements;

		SDL_Surface* img  = nullptr;
		SDL_Surface* imgBg = nullptr;
		SDL_Texture* text = nullptr;
};

