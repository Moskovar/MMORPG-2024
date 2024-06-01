#include "AutoAttack.h"



AutoAttack::AutoAttack(SDL_Renderer* renderer) : Spell("Auto attack")
{
    string src = "";
    for (float i = 0; i < 4; i += 0.5)
    {
        for (int j = 0; j < 22; j++)
        {
            if (i == 0) src = "img/entity/character/warrior/back/aa/";
            else if (i == 0.5 || i == 1 || i == 1.5) src = "img/entity/character/warrior/right/aa/";
            else if (i == 2) src = "img/entity/character/warrior/front/aa/";
            else if (i == 2.5 || i == 3 || i == 3.5) src = "img/entity/character/warrior/left/aa/";

            src += to_string(j + 1) + ".png";
            img[i][j] = IMG_Load(src.c_str());

            if (img[i][j]) text[i][j] = SDL_CreateTextureFromSurface(renderer, img[i][j]);
            else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

            if ((i == 0 || i == 2) && j == 16) break;
        }
    }
}

void AutoAttack::run(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& self, Entity* enemy)
{
    self.setAnimationID(this->animationID);
    self.setAAActive(true);
    for (int i = 0; i < 17; i++)
    {
        for(int p = 0; p < 25; p++)
        {
            if (self.getCancelAA()) { self.setAAActive(false); return; }
            Sleep(1);
        }
        //cout << e.step << endl;
        self.setStep((i % 21) * self.getAnimationSpeed());
        if ((self.getDir() == 0 || self.getDir() == 2) && i == 16) break; else self.increaseStep();
    }
    if (self.isMoving()) self.setAnimationID(1);
    else                 self.setAnimationID(0);
    self.setStep(0);
}

void AutoAttack::runOthers(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy)
{
}

void AutoAttack::resetSpell(Entity& player)
{
}
