#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <map>
#include <iostream>
#include <vector>

#include "uti.h"
#include "Element.h"
#include "Pseudo.h"
#include "Whirlwind.h"
#include "AutoAttack.h"
//faire un fichier avec tous les includes par classe de personnage pour ne pas avoir à tout include ??

#define IMG_SIZE 16
using namespace std;

class Entity : public Element
{
	public:
		Entity() {}
		Entity(std::string name, int category, SDL_Window* window, SDL_Renderer* renderer, string src);
		~Entity();

		static enum TYPE {
			Humanoid,
			Beast,
			Drake
		};

		static const map<short, string> types;

		virtual void draw(SDL_Renderer* renderer) = 0;

		void move(vector<Element*>* v_elements, bool& cameraLock);
		Spell* getSpell(int i) { return spells[i]; }

		Pseudo		 getPseudo()      { return this->pseudo;											  }
		float		 getPseudoX()     { return this->x + 125 - this->pseudo.getWidth() / 2;			      }
		float		 getPseudoY()     { return this->y + 5;											      }
		string		 getCategory()    { return uti::categories[uti::Language::FR][uti::Category::PLAYER]; }
		short		 getStep()        { return this->step;												  }
		SDL_Rect	 getPos()         { return this->pos;												  }
		float      	 getX()		      { return this->x;												      }
		float      	 getY()		      { return this->y;												      }
		short getXHitbox() { return xHitbox; }
		short getYHitbox() { return yHitbox; }
		float      	 getDir()		  { return this->dir;												  }
		short      	 getCountDir()    { return this->countDir;											  }
		float      	 getSpeed()	      { return this->speed;												  }
		float      	 getXRate()       { return this->xRate;												  }
		float      	 getYRate()       { return this->yRate;												  }
		short      	 getAnimationID() { return this->animationID;										  }
		bool  	     isAlive()		  { return this->alive;											      }
		bool      	 isMoving()		  { return this->moving;											  }
		bool      	 isSpellActive()  { return this->spellActive;										  }
		bool      	 isAAActive()	  { return this->aaActive;											  }
		bool		 getCancelAA()    { return cancelAA;												  }
		SDL_Texture* getTexture()	  { return text[animationID][dir][step];							  }

		void updateHitbox() { xHitbox = x + 125; yHitbox = y + 185; }
		void setX(float x) { this->x = x; }
		void setY(float y) { this->y = y; }
		void increaseX() { this->x++; }
		void increaseY() { this->y++; }
		void increaseX(float x) { this->x += x; this->pos.x = this->x; }
		void increaseY(float y) { this->y += y; this->pos.y = this->y; }
		void setPosX(int x) { this->pos.x = x; }
		void setPosY(int y) { this->pos.y = y; }
		void addXOffset(int xOffset) { this->xOffset += xOffset; }
		void addYOffset(int yOffset) { this->yOffset += yOffset; }
		void setStep(short step) { this->step = step; }
		void increaseStep() { this->step++; }
		void setAlive(bool alive)   { this->alive = alive;   }
		void setMoving(bool state) { this->moving = state; }
		void update();
		void setAAActive(bool state) { this->aaActive = state; }
		void setCancelAA(bool state) { this->cancelAA = state; }
		void setAnimationID(int animationID) { this->animationID = animationID; }
		void setSpellActive(bool state) { this->spellActive = state; }

		short countDir;
		bool up = false, right = false, down = false, left = false;

	protected:
		Pseudo pseudo;

		short category, step, xHitbox, yHitbox;
		float dir, xRate, yRate, speed;
		bool alive, moving, spellActive, aaActive, cancelAA;

		short animationID = 0;
		map <short, map<float, SDL_Surface*[30]>> img;
		map <short, map<float, SDL_Texture*[30]>> text;


		Spell* spells[4];
};