#include "Push.h"

Push::Push(SDL_Renderer* renderer) : Spell("Push")
{
    id = uti::SpellID::PUSH;
    cd = 1000;
    cancelable = false;
    moving = true;
    boostSpeed = 1;

    string src = "";
    for (float i = 0; i < 4; i += 0.5)
    {
        for (int j = 0; j < 12; j++)
        {
            if (i == 0)                              src = "img/entity/character/warrior/back/run/";
            else if (i == 0.5 || i == 1 || i == 1.5) src = "img/entity/character/warrior/right/run/";
            else if (i == 2)                         src = "img/entity/character/warrior/front/run/";
            else if (i == 2.5 || i == 3 || i == 3.5) src = "img/entity/character/warrior/left/run/";

            src += to_string(j + 1) + ".png";
            img[i][j] = IMG_Load(src.c_str());

            if (img[i][j]) text[i][j] = SDL_CreateTextureFromSurface(renderer, img[i][j]);
            else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

            if (i == 0 && j == 9)                                                              break;
            if ((i == 0.5 || i == 1 || i == 1.5 || i == 2.5 || i == 3 || i == 3.5) && j == 11) break;
            if (i == 2 && j == 8)                                                              break;
        }
    }
}

void Push::run(Entity& player)
{
    if (step == 0)
    {
        player.setAnimationID(id);
        player.setAnimationSpeed(10);
        player.setStaticSpellTarget(player.getTarget()->getCenterBox().center);

        moveX = 0.005 * (player.getStaticSpellTarget().x - player.getCenterBox().center.x);
        moveY = 0.005 * (player.getStaticSpellTarget().y - player.getCenterBox().center.y);
    }

    step++;
    player.setStep((step % 8) * player.getAnimationSpeed());

    if(player.getCenterBox().center.x >= player.getStaticSpellTarget().x - 5 && player.getCenterBox().center.x <= player.getStaticSpellTarget().x + 5
    && player.getCenterBox().center.y >= player.getStaticSpellTarget().y - 5 && player.getCenterBox().center.y <= player.getStaticSpellTarget().y + 5)
    {
        step = 0;
        player.setStep(0);
        player.setSpell();
        player.setAnimationSpeed(15);
        player.setStaticSpellTarget({ 0, 0 });
    }
}

void Push::runOthers(Entity& player)
{
    if (step == 0)
    {
        player.setAnimationID(id);
        player.setAnimationSpeed(10);
        player.setStaticSpellTarget(player.getTarget()->getCenterBox().center);

        moveX = 0.005 * (player.getStaticSpellTarget().x - player.getCenterBox().center.x);
        moveY = 0.005 * (player.getStaticSpellTarget().y - player.getCenterBox().center.y);
    }

    //player.addX(moveX);
    //player.addY(moveY);
    step++;
    player.setStep((step % 8) * player.getAnimationSpeed());
}

void Push::resetSpell(Entity& player)
{
    step = 0;
    player.setStep(0);
    player.setSpell();
    player.setAnimationSpeed(15);
    player.setStaticSpellTarget({ 0, 0 });
}

bool Push::isInRange(uti::Circle player, uti::Circle enemy)
{
    short distance = abs(player.center.x - enemy.center.x) + abs(player.center.y - enemy.center.y);
    return  distance > 400 && distance < 1000;
}

void Push::update(Entity& player)
{
    player.setXRate(moveX);
    player.setYRate(moveY);
}
