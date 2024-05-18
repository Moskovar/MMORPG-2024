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

void AutoAttack::run(vector<Element*>& v_elements, Entity& e, vector<MapFragment*>& v_mapFragments, bool& cameraLock)
{
    cout << e.getPos().x << endl;
    e.setAnimationID(this->animationID);
    e.setAAActive(true);
    for (int i = 0; i < 17; i++)
    {
        for(int p = 0; p < 25; p++)
        {
            if (e.getCancelAA()) { e.setAAActive(false); return; }
            Sleep(1);
        }
        //cout << e.step << endl;
        e.setStep(i % 21);
        if ((e.getDir() == 0 || e.getDir() == 2) && i == 16) break; else e.increaseStep();
    }
    if (e.isMoving()) e.setAnimationID(1);
    else              e.setAnimationID(0);
    e.setStep(0);
}
