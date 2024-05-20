#include "Whirlwind.h"
#include "Building.h"

Whirlwind::Whirlwind(SDL_Renderer* renderer) : Spell("Whirlwind")
{
    string src = "";
    for (float i = 0; i < 4; i += 0.5)
    {
        for (int j = 0; j < 20; j++)
        {
            if (i == 0) src = "img/entity/character/warrior/back/whirlwind/";
            else if (i == 0.5 || i == 1 || i == 1.5) src = "img/entity/character/warrior/right/whirlwind/";
            else if (i == 2) src = "img/entity/character/warrior/front/whirlwind/";
            else if (i == 2.5 || i == 3 || i == 3.5) src = "img/entity/character/warrior/left/whirlwind/";

            src += to_string(j + 1) + ".png";
            img[i][j] = IMG_Load(src.c_str());

            if (img[i][j]) text[i][j] = SDL_CreateTextureFromSurface(renderer, img[i][j]);
            else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

        }
    }
}

void Whirlwind::run(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& self, Entity* enemy, Map* m, bool& cameraLock, mutex* mtx)
{
    Uint32 lastTime = SDL_GetTicks64();
    Uint32 currentTime;
    float deltaTime;
    self.setSpellActive(true);
    self.setAnimationID(animationID);
    self.setStep(0);
    int step = 0;
    for (int i = 0; i < 100; i++)
    {   
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        mtx->lock();
        self.setStep((i % 20) * self.getANIMATIONMULTIPL());

        float xChange = self.getSpeed() * uti::pixDir[self.getDir()].xRate * deltaTime,
              yChange = self.getSpeed() * uti::pixDir[self.getDir()].yRate * deltaTime;

        self.setXChange(xChange);
        self.setYChange(yChange);
        
        bool collision = false;
        for (Element* b : v_elements_solid) if (b->check_collisions(self.getXMovebox() + xChange, self.getYMovebox() + yChange)) { collision = true; break; }
        if (!collision)
        {
            for (unsigned int i = 0; i < v_elements.size(); i++)
            {
                v_elements[i]->addXOffset(-xChange);
                v_elements[i]->addYOffset(-yChange);
            }

            m->addOffset(-xChange, -yChange);

            if (!cameraLock)
            {
                self.addX(xChange);
                self.addY(yChange);

                self.addXOffset(-xChange);//On déplace le personnage dans un sens et le offset dans l'autre pour faire le reset de la pos
                self.addYOffset(-yChange);//On déplace le personnage dans un sens et le offset dans l'autre pour faire le reset de la pos
            }
            else
            {
                for (Element* e : v_elements) e->resetPos();//applique le offset aux elements du décors
                m->resetPos();
            }

            self.addXMap(xChange);
            self.addYMap(yChange);
        }
        self.updateMovebox();
        self.updateClickBox();
        mtx->unlock();
        Sleep(5);
    }
    if (self.isMoving()) self.setAnimationID(1);
    else          self.setAnimationID(0);
    self.setStep(0);
    self.setSpellActive(false);
    self.setCancelAA(false);
}
