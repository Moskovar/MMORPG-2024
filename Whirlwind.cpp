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

void Whirlwind::run(vector<Element*>& v_elements, Entity& e, Map& m, bool& cameraLock, mutex* mtx)
{
    Uint32 lastTime = SDL_GetTicks64();
    Uint32 currentTime;
    float deltaTime;
    e.setSpellActive(true);
    e.setAnimationID(animationID);
    e.setStep(0);
    int step = 0;
    for (int i = 0; i < 100; i++)
    {   
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        mtx->lock();
        e.setStep((i % 20) * e.getANIMATIONMULTIPL());
        float xChange = e.getSpeed() * uti::pixDir[e.getDir()].xRate * deltaTime,
              yChange = e.getSpeed() * uti::pixDir[e.getDir()].yRate * deltaTime;
        
        bool collision = false;
        for (Element* b : v_elements) if (b->check_collisions(e.getXMovebox() + xChange, e.getYMovebox() + yChange)) { collision = true; break; }
        if (!collision)
        {
            for (unsigned int i = 0; i < v_elements.size(); i++)
            {
                v_elements[i]->addXOffset(-xChange);
                v_elements[i]->addYOffset(-yChange);
            }

            m.addOffset(-xChange, -yChange);

            if (!cameraLock)
            {
                e.addX(xChange);
                e.addY(yChange);

                e.addXOffset(-xChange);//On déplace le personnage dans un sens et le offset dans l'autre pour faire le reset de la pos
                e.addYOffset(-yChange);//On déplace le personnage dans un sens et le offset dans l'autre pour faire le reset de la pos
            }
            else
            {
                for (Element* e : v_elements) e->resetPos();//applique le offset aux elements du décors
                m.resetPos();
            }

            e.addXMap(xChange);
            e.addYMap(yChange);
        }
        e.updateMovebox();
        e.updateClickBox();
        mtx->unlock();
        Sleep(5);
    }
    if (e.isMoving()) e.setAnimationID(1);
    else          e.setAnimationID(0);
    e.setStep(0);
    e.setSpellActive(false);
    e.setCancelAA(false);
}
