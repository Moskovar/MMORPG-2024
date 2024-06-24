#include "Spell.h"

Spell::Spell(string name)
{
    this->name = name;
    this->step = 0;
}

void Spell::update(Entity& p)
{
}

void Spell::check_cd()
{
    long cd_left = chrono::duration_cast<std::chrono::milliseconds>(chrono::high_resolution_clock::now() - last_time_used).count();
    if (!available && cd_left >= cd)
    {
        available = true;
        spell_icon.setTimer(0);
        spell_icon.setCDIcon(false);
    }
    else if(!available)
    {
        spell_icon.setTimer(((cd - cd_left) / 1000) + 1);
    }
}

void Spell::draw(SDL_Renderer* renderer)
{
    spell_icon.draw(renderer);
}
