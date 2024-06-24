#include "AutoAttack.h"



AutoAttack::AutoAttack(SDL_Renderer* renderer) : Spell("Auto attack")
{
    id = uti::SpellID::AA;
    cd = 1000;

    string src = "";
    for (float i = 0; i < 4; i += 0.5)
    {
        for (int j = 0; j < 17; j++)
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

void AutoAttack::run(Entity& player)
{
    player.setDir(1);
    if (step == 0)
    {
        player.setAnimationID(id);
        player.setAnimationSpeed(10);
    }

    step++;
    player.setStep(step);

    if (step == 24 * player.getAnimationSpeed())
    {
        step = 0;
        player.setStep(0);
        player.setSpell();
        player.setAnimationSpeed(15);
    }
}

void AutoAttack::runOthers(Entity& player)
{
    if (!player.isAAActive())
    {
        step = 0;
        player.setAAActive(true);
        player.setAnimationID(uti::SpellID::AA);
        player.setAnimationSpeed(10);
    }

    step++;
    player.setStep(step);
    //cout << player.getAnimationID() << " : " << player.getStep() << " : " << player.getDir() << endl;
}

void AutoAttack::resetSpell(Entity& player)//why ici ça marche alors que si dans runOthers ça bug ??
{
    step = 0;
    player.setStep(0);
    player.setSpell();
    player.setAAActive(false);
    player.setAnimationSpeed(15);
}

bool AutoAttack::isInRange(uti::Circle player, uti::Circle enemy)
{
    return (abs(player.center.x - enemy.center.x) + abs(player.center.y - enemy.center.y)) <= 100;
}
