#include "Warrior.h"

Warrior::Warrior(std::string name, float xMap, float yMap, int id, int category, SDL_Renderer* renderer, bool drawUI) : Character(name, xMap, yMap, id, category, "warrior", renderer)
{
	this->drawUI = drawUI;

    spells[uti::SpellID::WHIRLWIND] = new Whirlwind(renderer, drawUI);
    img[uti::SpellID::WHIRLWIND] = spells[uti::SpellID::WHIRLWIND]->getImg();
    text[uti::SpellID::WHIRLWIND] = spells[uti::SpellID::WHIRLWIND]->getText();

    spells[uti::SpellID::PUSH] = new Push(renderer, drawUI);
    img[uti::SpellID::PUSH] = spells[uti::SpellID::PUSH]->getImg();
    text[uti::SpellID::PUSH] = spells[uti::SpellID::PUSH]->getText();

    spells[uti::SpellID::BLOODFURY] = new BloodFury(renderer, drawUI);
    img[uti::SpellID::BLOODFURY] = spells[uti::SpellID::BLOODFURY]->getImg();//useless ????? img change pas
    text[uti::SpellID::BLOODFURY] = spells[uti::SpellID::BLOODFURY]->getText();//useless ?????

	updateBoxes();
}
