#include "AutoAttack.h"



AutoAttack::AutoAttack(SDL_Renderer* renderer) : Spell("Auto attack")
{
    id = uti::SpellID::AA;
    cd = 1000;
}

void AutoAttack::run(Entity& player)
{
    player.setDir(0.5);
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
