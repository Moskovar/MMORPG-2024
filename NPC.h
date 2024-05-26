#pragma once
#include "Entity.h"
#include "Quest.h"
#include "Character.h"

class NPC : public Entity
{
	public:
		NPC() {}
		NPC(std::string name, float xMap, float yMap, int id, int category, string className, bool isSolid, Character* c, SDL_Renderer* renderer);// le character sert de référentiel pour attribuer le x y au NPC

		Quest* getPQuest() { return &this->quest; }


		void updateXOffset(float cameraSpeed) override { x += cameraSpeed; pos.x = x; xOffset -= cameraSpeed; quest.iconPos.x = this->x + 125 - 17 / 2; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
		void updateYOffset(float cameraSpeed) override { y += cameraSpeed; pos.y = y; yOffset -= cameraSpeed; quest.iconPos.y = this->y - 25; }//On envoie le personnage dans un sens et on met le offset dans l'autre pour le retirer ensuite
		void resetPos() override;
		void draw(SDL_Renderer* renderer);
	private:
		bool hasQuest;
		Quest quest;

};

