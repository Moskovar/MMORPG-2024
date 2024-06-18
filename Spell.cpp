#include "Spell.h"

Spell::Spell(string name)
{
    this->name = name;
    this->step = 0;
}

void Spell::update(Entity& p)
{
}

void Spell::draw(SDL_Renderer* renderer)
{
    spell_icon.draw(renderer);
}
