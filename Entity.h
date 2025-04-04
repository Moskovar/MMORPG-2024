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
#include "Push.h"
#include "BloodFury.h"
//faire un fichier avec tous les includes par classe de personnage pour ne pas avoir � tout include ??

#define IMG_SIZE 16
using namespace std;

struct SpellEffect //� bouger dans uti
{
	short entityID = 0;
	short spellID = 0;
	chrono::high_resolution_clock::time_point last_hit_time;
};

class Entity : public Element
{
	public:
		Entity() {}
		Entity(std::string name, float x, float y, int id, short faction, string src, SDL_Renderer* renderer);
		~Entity();

		static const map<short, string> types;

		virtual void draw(SDL_Renderer* renderer) = 0;

		void move(vector<Element*>& v_elements, vector<Element*>& v_elements_solid, Map& m, bool& cameraLock, float& deltaTime, bool& sendSpellData, vector<SpellEffect>& spellEffects, chrono::high_resolution_clock::time_point now);

		Spell* getSpell(int i)			  { return spells[i];												  }
		Spell* getSpellUsed() { return this->spellUsed; }
		map<short, Spell*>* getSpells() { return &this->spells; }
		Pseudo		 getPseudo()          { return this->pseudo;											  }
		float		 getPseudoX()         { return this->x + 125 - this->pseudo.getWidth() / 2;			      }
		float		 getPseudoY()         { return this->y + 15;											  }
		string		 getCategory()        { return uti::categories[uti::Language::FR][uti::Category::PLAYER]; }
		short		 getStep();
		short		 getFlatStep()		  { return this->step;												  }
		SDL_Rect	 getPos()             { return this->pos;												  }
		float      	 getX()		          { return this->x;												      }
		float      	 getY()		          { return this->y;												      }
		short		 getXMovebox()        { return xMovebox;												  }
		short		 getYMovebox()        { return yMovebox;												  }
		short getXCenterBox() { return this->centerBox.center.x; }
		short getYCenterBox() { return this->centerBox.center.y; }
		short getCenterBoxRadius() { return this->centerBox.radius; }
		uti::Circle getCenterBox() { return this->centerBox; }
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
		//bool      	 isSpellActive()      { return this->spellActive;										  }
		bool      	 isAAActive()	      { return this->aaActive;											  }
		bool		 getCancelAA()        { return cancelAA;												  }
		SDL_Texture* getPortraitTexture() { return textPortrait;											  }
		SDL_Texture* getTexture()		  { return text[animationID][dir][step / animationSpeed];			  }
		float getDeltaTime()			  { return this->deltaTime;											  } //pour debug
		float getXChange()				  { return this->xChange;											  }
		float getYChange()				  { return this->yChange;											  }	
		short getHealth() { return this->health; }
		short getFaction() { return this->faction; }
		Entity* getTarget() { return target; }
		uti::Point getStaticSpellTarget() { return staticSpellTarget; }
		bool isInGoodDirection();
		


		void setAnimationSpeed(short animationSpeed) { this->animationSpeed = animationSpeed; }
		void setHealth(int health);
		void takeDamages(short dmg);
		void updateRBars();
		void increaseX()					 { this->x++; this->pos.x = x;							   }
		void increaseStep()					 { step++;												   }
		void resetStep()					 { step = 0;											   }
		void updateMovebox()				 { xMovebox = x + 135; yMovebox = y + 235;				   }
		void updateClickBox()				 { clickBox.x = x + 100;  clickBox.y = y + 90;	   }
		void updateCenterBox()				 { centerBox.center.x = x + 140; centerBox.center.y = y + 180;     }
		void updateBoxes()					 { updateMovebox(); updateClickBox(); updateRBars(); updateCenterBox(); }
		void addX(float x)				     { this->x += x; this->pos.x = this->x;					   }
		void addY(float y)				     { this->y += y; this->pos.y = this->y;					   }
		void addXOffset(int xOffset)		 { this->xOffset += xOffset;							   }
		void addYOffset(int yOffset)		 { this->yOffset += yOffset;							   }
		void setStep(short step)			 { this->step = step;									   }
		void setAlive(bool alive)			 { this->alive = alive;									   }
		void setMoving(bool state)			 { this->moving = state;								   }
		void update();																				   
		void setAAActive(bool state)         { this->aaActive = state; spells[uti::SpellID::AA]->resetStep(); }
		void setCancelAA(bool state)		 { this->cancelAA = state;								   }
		void setAnimationID(int animationID) { this->animationID = animationID;						   }
		//void setSpellActive(bool state)      { this->spellActive = state;							   }
		void setXChange(float xChange)		 { this->xChange = xChange;								   }
		void setYChange(float yChange)		 { this->yChange = yChange;								   }
		void setXRate(float xRate) { this->xRate = xRate; }
		void setYRate(float yRate) { this->yRate = yRate; }
		void setSpell(short spellID);
		void setSpell() { this->spellUsed = nullptr; }
		void setFaction(short faction) { this->faction = faction; }
		void setHealthImg(short playerFaction, SDL_Renderer* renderer);
		void setTarget(Entity* target) { this->target = target; }
		void resetTarget() { this->target = nullptr; }
		void setStaticSpellTarget(uti::Point p) { this->staticSpellTarget = p; }
		void setDir(float dir) { this->dir = dir; }
		void scaleSize(float rate) { pos.w *= rate; pos.h *= rate; }

		void setPos(float x, float y);

		//--- M�thode pour la NetworkEntity ---//
		uti::NetworkEntity getNE() { return {0, id, countDir, health, (int)xMap * 100, (int)yMap * 100, uti::getCurrentTimestamp()}; }
		uti::NetworkEntitySpell getNES(short spellID) { return {1, id, spellID}; }//r�cup�rer le spell id autrement
		uti::NetworkEntitySpellEffect getNESE(short spellID, short eid) { return { 2, eid, spellID }; }

		short countDir;
		bool up = false, right = false, down = false, left = false;

		bool isInClickRange(int x, int y) { return (abs(this->xMovebox - x) + abs(this->yMovebox - y)) < 100; }
		bool isInFront(int x, int y) override;
		bool check_collisions(int x, int y) override;

	protected:
		short id = 0, health = 0, animationSpeed = 15, animationPause = 0;

		Pseudo pseudo;
		SDL_Surface* imgPortrait   = nullptr;
		SDL_Texture* textPortrait  = nullptr;

		SDL_Surface* imgBar		   = nullptr;
		SDL_Texture* textBar	   = nullptr;
		SDL_Surface* imgHealth     = nullptr;
		SDL_Texture* textHealth    = nullptr;

		SDL_Surface* imgRessource  = nullptr;
		SDL_Texture* textRessource = nullptr;

		SDL_Rect posBarH = { 0, 0, 0, 0 }, posBarR = { 0, 0, 0, 0 }, posHealth = { 0, 0, 0, 0 }, posRessource = { 0, 0, 0, 0 };

		float deltaTime = 0;//a delete pour debug avec la position anticip�e
		float xChange = 0, yChange = 0;

		short category = 0, step = 0, xMovebox = 0, yMovebox = 0, faction = 0;//faction 0 1 2 neutral fac1 fac2
		uti::Circle centerBox = { { 0, 0 }, 0 };
		float dir = 0.0f, xRate = 0.0f, yRate = 0.0f, speed = 0.0f;
		bool alive = true, moving = false/*, spellActive = false*/, aaActive = false, cancelAA = false, drawUI = false;

		short animationID = 0;

		map <short, map<float, SDL_Surface*[30]>> img;
		map <short, map<float, SDL_Texture*[30]>> text;

		map <short, map<float, SDL_Surface*>> imgHair;
		map <short, map<float, SDL_Texture*>> textHair;

		map <short, map<float, SDL_Surface*>> imgHead;
		map <short, map<float, SDL_Texture*>> textHead;

		map <short, map<float, SDL_Surface*>> imgBody;
		map <short, map<float, SDL_Texture*>> textBody;

		map <short, map<float, SDL_Surface*>> imgArmL;
		map <short, map<float, SDL_Texture*>> textArmL;

		map <short, map<float, SDL_Surface*>> imgArmR;
		map <short, map<float, SDL_Texture*>> textArmR;

		map <short, map<float, SDL_Surface*>> imgFull;
		map <short, map<float, SDL_Texture*>> textFull;

		SDL_Rect clickBox{ 0, 0, 0, 0 };

		Entity* target = nullptr;
		uti::Point staticSpellTarget = { 0, 0 };
		map<short, Spell*> spells;
		Spell* spellUsed;

		
};