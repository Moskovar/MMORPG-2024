#include "Whirlwind.h"
#include "Building.h"

Whirlwind::Whirlwind(SDL_Renderer* renderer) : Spell("Whirlwind")
{
    moving = true;
    boostSpeed = 2;
    range = 50;
    dmg = 10;

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

void Whirlwind::run(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy)
{
    if (!player.isSpellActive())
    {
        cout << "OK START" << endl;
        player.setSpellActive(true);
        player.setAnimationID(animationID);
        player.setAnimationSpeed(20);
    }

    step++;
    player.setStep((step % 20) * player.getAnimationSpeed());
    if (step % 20 == 0) cout << player.getAnimationID() << endl;//cout << player.getDir() << " : " << step << endl;

    if (step == 500)
    {
        step = 0;
        player.setStep(0);
        player.setSpell();
        player.setSpellActive(false);
        player.setCancelAA(false);
        player.setAnimationSpeed(15);
    }
}

void Whirlwind::runOthers(vector<Element*>& v_elements, vector<Element*> v_elements_solid, Entity& player, Entity* enemy)
{
    if (!player.isSpellActive())
    {
        cout << "OK START" << endl;
        player.setSpellActive(true);
        player.setAnimationID(animationID);
        player.setAnimationSpeed(20);
    }

    step++;
    player.setStep((step % 20) * player.getAnimationSpeed());
    if (step % 20 == 0) cout << player.getAnimationID() << endl;//cout << player.getDir() << " : " << step << endl;
}

void Whirlwind::resetSpell(Entity& player)
{
    step = 0;
    player.setStep(0);
    player.setSpellActive(false);
    player.setCancelAA(false);
    player.setAnimationSpeed(15);
}

bool Whirlwind::isInRange(uti::Circle player, uti::Circle enemy)
{
    return uti::doCirclesIntersect(player, enemy);
}
