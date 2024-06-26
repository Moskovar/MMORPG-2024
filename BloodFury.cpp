#include "BloodFury.h"

BloodFury::BloodFury(SDL_Renderer* renderer, bool drawUI) : Spell("BloodFury")
{
    if (drawUI) spell_icon = SpellIcon("warrior", this, 2, renderer);
    id         = uti::SpellID::BLOODFURY;
    cd         = 60;
    cancelable = false;
    moving     = false;
    boostSpeed = 1.5;
}

void BloodFury::run(Entity& player)
{
    player.setX(player.getX() - 250 * 0.2 / 2);
    player.setY(player.getY() - 250 * 0.2);
    player.scaleSize(1.2);
    player.setSpell();

    cout << player.getPos().w << " : " << player.getPos().h << endl;

    //if (step == 0)
    //{
    //    player.setAnimationID(id);
    //    player.setAnimationSpeed(20);
    //}

    //step++;
    //player.setStep((step % 20) * player.getAnimationSpeed());
    ////if (step % 20 == 0) cout << player.getAnimationID() << endl;//cout << player.getDir() << " : " << step << endl;

    //if (step == 500)
    //{
    //    step = 0;
    //    player.setStep(0);
    //    player.setSpell();
    //    player.setCancelAA(false);
    //    player.setAnimationSpeed(15);
    //}
}

void BloodFury::runOthers(Entity& player)
{
}

void BloodFury::resetSpell(Entity& player)
{
}

bool BloodFury::isInRange(uti::Circle player, uti::Circle enemy)
{
    return false;
}

void BloodFury::update(Entity& p)
{
}
