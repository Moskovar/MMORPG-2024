#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <mutex>

#include "uti.h"
#include "Map.h"
#include "Pseudo.h"
#include "Whirlwind.h"
#include "AutoAttack.h"
//faire un fichier avec tous les includes par classe de personnage pour ne pas avoir à tout include ??

#define IMG_SIZE 16
#define ANIMATIONMULTIPL 15
using namespace std;

class Entity : public Element
{
	public:
		Entity() {}
		Entity(std::string name, float x, float y, int category, string src, SDL_Renderer* renderer);
		~Entity();

		static enum TYPE {
			Humanoid,
			Beast,
			Drake
		};

		static const map<short, string> types;

		virtual void draw(SDL_Renderer* renderer) = 0;

		void move(vector<Element*>& v_elements, Map& m, bool& cameraLock, float& deltaTime);

		Spell* getSpell(int i)			  { return spells[i];												  }
		Pseudo		 getPseudo()          { return this->pseudo;											  }
		float		 getPseudoX()         { return this->x + 125 - this->pseudo.getWidth() / 2;			      }
		float		 getPseudoY()         { return this->y + 35;											      }
		string		 getCategory()        { return uti::categories[uti::Language::FR][uti::Category::PLAYER]; }
		short		 getStep()            { return this->step / ANIMATIONMULTIPL;												  }
		SDL_Rect	 getPos()             { return this->pos;												  }
		float      	 getX()		          { return this->x;												      }
		float      	 getY()		          { return this->y;												      }
		short		 getXMovebox()        { return xMovebox;												  }
		short		 getYMovebox()        { return yMovebox;												  }
		SDL_Rect*    getPClickBox()       { return &clickBox;												  }
		float      	 getDir()		      { return this->dir;												  }
		short      	 getCountDir()        { return this->countDir;											  }
		float      	 getSpeed()	          { return this->speed;												  }
		float      	 getXRate()           { return this->xRate;												  }
		float      	 getYRate()           { return this->yRate;												  }
		int			 getANIMATIONMULTIPL(){ return ANIMATIONMULTIPL;										  }
		bool		 inClickBox(int x, int y);
		short      	 getAnimationID()     { return this->animationID;										  }
		bool  	     isAlive()		      { return this->alive;											      }
		bool      	 isMoving()		      { return this->moving;											  }
		bool      	 isSpellActive()      { return this->spellActive;										  }
		bool      	 isAAActive()	      { return this->aaActive;											  }
		bool		 getCancelAA()        { return cancelAA;												  }
		SDL_Texture* getPortraitTexture() { return textPortrait;											  }
		SDL_Texture* getTexture()		  { return text[animationID][dir][step / ANIMATIONMULTIPL];							  }

		void increaseX() { this->x++; this->pos.x = x; }
		void		 updateMovebox()				 { xMovebox = x + 125; yMovebox = y + 185;				  }
		void		 updateClickBox()				 { clickBox.x = pos.x + 90; clickBox.y = pos.y + 65;	  }
		void		 addX(float x)				     { this->x += x; this->pos.x = this->x;					  }
		void		 addY(float y)				     { this->y += y; this->pos.y = this->y;					  }
		void		 addXOffset(int xOffset)		 { this->xOffset += xOffset;							  }
		void		 addYOffset(int yOffset)		 { this->yOffset += yOffset;							  }
		void		 setStep(short step)			 { this->step = step;									  }
		void		 increaseStep()					 { this->step++;										  }
		void		 setAlive(bool alive)			 { this->alive = alive;									  }
		void		 setMoving(bool state)			 { this->moving = state;								  }
		void		 update();																				  
		void		 setAAActive(bool state)		 { this->aaActive = state;								  }
		void		 setCancelAA(bool state)		 { this->cancelAA = state;								  }
		void		 setAnimationID(int animationID) { this->animationID = animationID;						  }
		void		 setSpellActive(bool state)      { this->spellActive = state;							  }
		

		short countDir;
		bool up = false, right = false, down = false, left = false;


		bool isInClickRange(int x, int y) { return (abs(this->xMovebox - x) + abs(this->yMovebox - y)) < 75; }
		bool isInFront(int x, int y) override;
		bool check_collisions(int x, int y) override;

	protected:
		Pseudo pseudo;
		SDL_Surface* imgPortrait  = nullptr;
		SDL_Texture* textPortrait = nullptr;

		short category, step, xMovebox, yMovebox;
		float dir, xRate, yRate, speed;
		bool alive, moving, spellActive, aaActive, cancelAA;

		short animationID = 0;
		map <short, map<float, SDL_Surface*[30]>> img;
		map <short, map<float, SDL_Texture*[30]>> text;

		SDL_Rect clickBox;

		Spell* spells[4];
};