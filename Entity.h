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
using namespace std;

class Entity : public Element
{
	public:
		Entity() {}
		Entity(std::string name, float x, float y, int id, int category, string src, SDL_Renderer* renderer);
		~Entity();

		static enum TYPE {
			Humanoid,
			Beast,
			Drake
		};

		static const map<short, string> types;

		virtual void draw(SDL_Renderer* renderer) = 0;

		void move(vector<Element*>& v_elements, vector<Element*>& v_elements_solid, Map& m, bool& cameraLock, float& deltaTime, bool& sendSpellData);

		Spell* getSpell(int i)			  { return spells[i];												  }
		Spell* getSpellUsed() { return this->spellUsed; }
		Pseudo		 getPseudo()          { return this->pseudo;											  }
		float		 getPseudoX()         { return this->x + 125 - this->pseudo.getWidth() / 2;			      }
		float		 getPseudoY()         { return this->y + 15;											  }
		string		 getCategory()        { return uti::categories[uti::Language::FR][uti::Category::PLAYER]; }
		short		 getStep()            { return this->step / animationSpeed;							  }
		short		 getFlatStep()		  { return this->step;												  }
		SDL_Rect	 getPos()             { return this->pos;												  }
		float      	 getX()		          { return this->x;												      }
		float      	 getY()		          { return this->y;												      }
		short		 getXMovebox()        { return xMovebox;												  }
		short		 getYMovebox()        { return yMovebox;												  }
		short		 getID()			  { return this->id;												  }
		SDL_Rect*    getPClickBox()       { return &clickBox;												  }
		float      	 getDir()		      { return this->dir;												  }
		float		 getSpeed()			  { return this->speed;												  }
		float		 getXRate()			  { return this->xRate;												  }
		float		 getYRate()			  { return this->yRate;												  }
		short      	 getCountDir()        { return this->countDir;											  }
		short		 getAnimationSpeed()  { return animationSpeed;						    				  }
		bool		 inClickBox(int x, int y);
		short      	 getAnimationID()     { return this->animationID;										  }
		bool  	     isAlive()		      { return this->alive;											      }
		bool      	 isMoving()		      { return this->moving;											  }
		bool      	 isSpellActive()      { return this->spellActive;										  }
		bool      	 isAAActive()	      { return this->aaActive;											  }
		bool		 getCancelAA()        { return cancelAA;												  }
		SDL_Texture* getPortraitTexture() { return textPortrait;											  }
		SDL_Texture* getTexture()		  { return text[animationID][dir][step / animationSpeed];			  }
		float getDeltaTime()			  { return this->deltaTime;											  } //pour debug
		float getXChange()				  { return this->xChange;											  }
		float getYChange()				  { return this->yChange;											  }	
		short getHealth() { return this->health; }


		void setAnimationSpeed(short animationSpeed) { this->animationSpeed = animationSpeed; }
		void setHealth(int health);
		void takeDamages(short dmg);
		void updateRBars();
		void increaseX()					 { this->x++; this->pos.x = x;							   }
		void increaseStep()					 { step++;												   }
		void resetStep()					 { step = 0;											   }
		void updateMovebox()				 { xMovebox = x + 125; yMovebox = y + 185;				   }
		void updateClickBox()				 { clickBox.x = pos.x + 90; clickBox.y = pos.y + 65;	   }
		void addX(float x)				     { this->x += x; this->pos.x = this->x;					   }
		void addY(float y)				     { this->y += y; this->pos.y = this->y;					   }
		void addXOffset(int xOffset)		 { this->xOffset += xOffset;							   }
		void addYOffset(int yOffset)		 { this->yOffset += yOffset;							   }
		void setStep(short step)			 { this->step = step;									   }
		void setAlive(bool alive)			 { this->alive = alive;									   }
		void setMoving(bool state)			 { this->moving = state;								   }
		void update();																				   
		void setAAActive(bool state)		 { this->aaActive = state;								   }
		void setCancelAA(bool state)		 { this->cancelAA = state;								   }
		void setAnimationID(int animationID) { this->animationID = animationID;						   }
		void setSpellActive(bool state)      { this->spellActive = state;							   }
		void setXChange(float xChange)		 { this->xChange = xChange;								   }
		void setYChange(float yChange)		 { this->yChange = yChange;								   }
		void setSpell(short spellID) { if (spellID == 0) { this->spellUsed->resetSpell(*this); this->spellUsed = nullptr; this->animationID = 1; return; }   this->spellUsed = spells[spellID]; }
		void setSpell() { this->spellUsed = nullptr; }

		void setPos(float x, float y);

		//--- Méthode pour la NetworkEntity ---//
		uti::NetworkEntity getNE() { return {0, id, countDir, health, (int)xMap * 100, (int)yMap * 100, uti::getCurrentTimestamp()}; }
		uti::NetworkEntitySpell getNES(short spellID) { return {1, id, spellID}; }

		short countDir;
		bool up = false, right = false, down = false, left = false;

		bool isInClickRange(int x, int y) { return (abs(this->xMovebox - x) + abs(this->yMovebox - y)) < 100; }
		bool isInFront(int x, int y) override;
		bool check_collisions(int x, int y) override;

	protected:
		short id = 0, health = 0, animationSpeed = 15;

		Pseudo pseudo;
		SDL_Surface* imgPortrait  = nullptr;
		SDL_Texture* textPortrait = nullptr;

		SDL_Surface* imgBar		   = nullptr;
		SDL_Texture* textBar	   = nullptr;
		SDL_Surface* imgHealth	   = nullptr;
		SDL_Texture* textHealth    = nullptr;
		SDL_Surface* imgRessource  = nullptr;
		SDL_Texture* textRessource = nullptr;

		SDL_Rect posBarH = { 0, 0, 0, 0 }, posBarR = { 0, 0, 0, 0 }, posHealth = { 0, 0, 0, 0 }, posRessource = { 0, 0, 0, 0 };

		float deltaTime = 0;//a delete pour debug avec la position anticipée
		float xChange = 0, yChange = 0;

		short category = 0, step = 0, xMovebox = 0, yMovebox = 0;
		float dir = 0.0f, xRate = 0.0f, yRate = 0.0f, speed = 0.0f;
		bool alive = true, moving = false, spellActive = false, aaActive = false, cancelAA = false;

		short animationID = 0;
		map <short, map<float, SDL_Surface*[30]>> img;
		map <short, map<float, SDL_Texture*[30]>> text;

		SDL_Rect clickBox{ 0, 0, 0, 0 };

		map<short, Spell*> spells;
		Spell* spellUsed;
};