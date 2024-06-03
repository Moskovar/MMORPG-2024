#include "Spell.h"

Spell::Spell(string name)
{
    this->name = name;
    this->step = 0;
}

void Spell::checkDmgDone()
{
    if (!dmgDone) return;//si les dmg n'ont pas été fait alors on sort, la suite est useless
    auto now = std::chrono::high_resolution_clock::now(); 
    if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last_dmg_time).count() >= hitSpeed)
    {
        dmgDone = false;
    }    
}
