#include "Character.h"

Character::Character(std::string name, float xMap, float yMap, int id, short faction, string src, SDL_Renderer* renderer) : Entity(name, xMap, yMap, id, faction, "character/" + src, renderer)
{
	clickBox.x = x + 90;
	clickBox.y = y + 65;
	clickBox.w = 75;
	clickBox.h = 160;
};

void Character::resetPos()
{
    x += xOffset;
    pos.x = x;
    y += yOffset;
    pos.y = y;
    xOffset = 0;
    yOffset = 0;

    updateMovebox();
    updateClickBox();
    updateRBars();
}

void Character::draw(SDL_Renderer* renderer)
{
	//pseudo.draw(renderer, getPseudoX(), getPseudoY());
    pseudo.draw(renderer, getPseudoX(), getPseudoY());
    updateRBars();
    SDL_RenderCopy(renderer, textHealth, NULL, &posHealth);
    SDL_RenderCopy(renderer, textBar, NULL, &posBarH);
	if(animationID == 3 || animationID == 4) if(text[animationID][dir][step / animationSpeed]) SDL_RenderCopy(renderer, text[animationID][dir][step / animationSpeed], NULL, &this->pos);

    showPos.x = 500 * ((step / animationSpeed) % 4);
    showPos.y = 500 * ((step / animationSpeed) / 4);
    showPos.w = 500;
    showPos.h = 500;
    
    //cout << step / animationSpeed / 4 << " : " << srcRect.x << " : " << srcRect.y << std::endl;

    //--- On dessine le bras derrière ---//
    if (dir == 1) if (textArmL[animationID][dir]) SDL_RenderCopy(renderer, textArmL[animationID][dir], &showPos, &pos);

    if (animationID == 3 && moving)
    {
        if (textBody[animationID][dir]) SDL_RenderCopy(renderer, textBody[1][dir], &showPos, &pos);
        if (textHead[animationID][dir]) SDL_RenderCopy(renderer, textHead[1][dir], &showPos, &pos);
    }
    else
    {
        if (textBody[animationID][dir]) SDL_RenderCopy(renderer, textBody[animationID][dir], &showPos, &pos);
        if (textHead[animationID][dir]) SDL_RenderCopy(renderer, textHead[animationID][dir], &showPos, &pos);
    }

    if (animationID == 3 && moving) { if (textHair[animationID][dir]) SDL_RenderCopy(renderer, textHair[1][dir], &showPos, &pos); }
    else                              if (textHair[animationID][dir]) SDL_RenderCopy(renderer, textHair[animationID][dir], &showPos, &pos);

    //--- On dessine le bras devant ---//
    if (dir == 1) if (textArmR[animationID][dir]) SDL_RenderCopy(renderer, textArmR[animationID][dir], &showPos, &pos);
    
    if(drawUI) for (auto it = spells.begin(); it != spells.end(); ++it) it->second->draw(renderer);
}
