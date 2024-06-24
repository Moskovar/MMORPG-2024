#include "Entity.h"
#include "Building.h"
#include <chrono>

Entity::Entity(std::string name, float xMap, float yMap, int id, short faction, string imgSrc, SDL_Renderer* renderer) : Element(xMap, yMap, 250, 250 )
{
    this->pseudo = Pseudo(name, renderer);

    this->id       = id;
    this->health   = 100;
    this->category = category;
    this->step     = 0;
    this->speed    = 400;
    this->dir      = 2;
    this->countDir = 0;

    this->alive       = true;
    this->moving      = false;
    //this->spellActive = false;
    this->aaActive    = false;
    this->cancelAA    = false;

    this->xRate = 0;
    this->yRate = 0;

    posBarH.x  = x;
    posBarH.y  = y;
    posBarH.w  = 100;
    posBarH.h  = 10;
    posHealth.h = 10;

    centerBox.radius = 30;

    imgBody[0][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/idle/spritesheet.png").c_str());
    if (imgBody[0][3]) textBody[0][3] = SDL_CreateTextureFromSurface(renderer, imgBody[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle left body.png"); exit(0); }



    //--- Right ---//

    //--- idle ---//
    imgHair[0][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/idle/hair.png").c_str());
    if (imgHair[0][1]) textHair[0][1] = SDL_CreateTextureFromSurface(renderer, imgHair[0][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right hair.png"); exit(0); }

    imgHead[0][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/idle/head.png").c_str());
    if (imgHead[0][1]) textHead[0][1] = SDL_CreateTextureFromSurface(renderer, imgHead[0][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right head.png"); exit(0); }

    imgBody[0][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/idle/body.png").c_str());
    if (imgBody[0][1]) textBody[0][1] = SDL_CreateTextureFromSurface(renderer, imgBody[0][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right body.png"); exit(0); }

    imgArmL[0][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/idle/arml.png").c_str());
    if (imgArmL[0][1]) textArmL[0][1] = SDL_CreateTextureFromSurface(renderer, imgArmL[0][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right arml.png"); exit(0); }

    imgArmR[0][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/idle/armr.png").c_str());
    if (imgArmR[0][1]) textArmR[0][1] = SDL_CreateTextureFromSurface(renderer, imgArmR[0][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right armr.png"); exit(0); }

    //--- run ---//

    imgHair[1][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/run/hair.png").c_str());
    if (imgHair[1][1]) textHair[1][1] = SDL_CreateTextureFromSurface(renderer, imgHair[1][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run right hair.png"); exit(0); }

    imgHead[1][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/run/head.png").c_str());
    if (imgHead[1][1]) textHead[1][1] = SDL_CreateTextureFromSurface(renderer, imgHead[1][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run right head.png"); exit(0); }

    imgBody[1][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/run/body.png").c_str());
    if (imgBody[1][1]) textBody[1][1] = SDL_CreateTextureFromSurface(renderer, imgBody[1][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run right body.png"); exit(0); }

    imgArmL[1][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/run/arml.png").c_str());
    if (imgArmL[1][1]) textArmL[1][1] = SDL_CreateTextureFromSurface(renderer, imgArmL[1][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run right arml.png"); exit(0); }

    imgArmR[1][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/run/armr.png").c_str());
    if (imgArmR[1][1]) textArmR[1][1] = SDL_CreateTextureFromSurface(renderer, imgArmR[1][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run right armr.png"); exit(0); }


    //--- auto attack ---//

    imgHair[3][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/aa/hair.png").c_str());
    if (imgHair[3][1]) textHair[3][1] = SDL_CreateTextureFromSurface(renderer, imgHair[3][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa right hair.png"); exit(0); }

    imgHead[3][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/aa/head.png").c_str());
    if (imgHead[3][1]) textHead[3][1] = SDL_CreateTextureFromSurface(renderer, imgHead[3][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa right head.png"); exit(0); }

    imgBody[3][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/aa/body.png").c_str());
    if (imgBody[3][1]) textBody[3][1] = SDL_CreateTextureFromSurface(renderer, imgBody[3][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa right body.png"); exit(0); }

    imgArmL[3][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/aa/arml.png").c_str());
    if (imgArmL[3][1]) textArmL[3][1] = SDL_CreateTextureFromSurface(renderer, imgArmL[3][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa right arml.png"); exit(0); }

    imgArmR[3][1] = IMG_Load(string("img/entity/" + imgSrc + "/right/aa/armr.png").c_str());
    if (imgArmR[3][1]) textArmR[3][1] = SDL_CreateTextureFromSurface(renderer, imgArmR[3][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa right armr.png"); exit(0); }






    imgPortrait = IMG_Load(string("img/entity/" + imgSrc + "/picture/head.png").c_str());
    if (imgPortrait) textPortrait = SDL_CreateTextureFromSurface(renderer, imgPortrait);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> portrait.png"); exit(0); }

    SDL_Surface* imgBar = IMG_Load(string("img/ui/ressources/bar.png").c_str());
    if (imgBar) textBar = SDL_CreateTextureFromSurface(renderer, imgBar);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> bar.png");      exit(0); }

    SDL_Surface* imgHealth = IMG_Load(string(string("img/ui/ressources/a_health.png")).c_str());
    if (imgHealth) textHealth = SDL_CreateTextureFromSurface(renderer, imgHealth);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> health.png");   exit(0); }

    SDL_Surface* imgRessource = IMG_Load(string("img/ui/ressources/mana.png").c_str());
    if (imgRessource) textRessource = SDL_CreateTextureFromSurface(renderer, imgRessource);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> mana.png");     exit(0); }

    string src = "";
    for (float i = 0; i < 4; i += 0.5)
    {
        if      (i == 0)   img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/back/stand/0.png").c_str());//mettre dans classe fille ou faire un truc générique qui charge dans la classe fille avec le chemin dédié à la classe
        else if (i == 0.5) img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/right/stand/0.png").c_str());
        else if (i == 1)   img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/right/stand/0.png").c_str());
        else if (i == 1.5) img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/right/stand/0.png").c_str());
        else if (i == 2)   img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/front/stand/0.png").c_str());
        else if (i == 2.5) img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/left/stand/0.png").c_str());
        else if (i == 3)   img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/left/stand/0.png").c_str());
        else if (i == 3.5) img[0][i][0] = IMG_Load(std::string("img/entity/" + imgSrc + "/left/stand/0.png").c_str());
        
        if (img[0][i][0]) text[0][i][0] = SDL_CreateTextureFromSurface(renderer, img[0][i][0]);
        else             { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + std::to_string(i)).c_str());   exit(0); }
        
        for (int j = 0; j < 12; j++)
        {
            if      (i == 0) src = "img/entity/" + imgSrc + "/back/run/";
            else if (i == 0.5 || i == 1 || i == 1.5) src = "img/entity/" + imgSrc + "/right/run/";
            else if (i == 2) src = "img/entity/" + imgSrc + "/front/run/";
            else if (i == 2.5 || i == 3 || i == 3.5) src = "img/entity/" + imgSrc + "/left/run/";

            src += to_string(j + 1) + ".png";
            img[1][i][j] = IMG_Load(src.c_str());

            if (img[1][i][j]) text[1][i][j] = SDL_CreateTextureFromSurface(renderer, img[1][i][j]);
            else              { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

            if ( i == 0 && j == 9)                                                              break;
            if ((i == 0.5 || i == 1 || i == 1.5 || i == 2.5 || i == 3 || i == 3.5) && j == 11)  break;
            if ( i == 2 && j == 8)                                                              break;
        }
    }

    spells[uti::SpellID::AA] = new AutoAttack(renderer);
    img[uti::SpellID::AA]    = spells[uti::SpellID::AA]->getImg();
    text[uti::SpellID::AA]   = spells[uti::SpellID::AA]->getText();

    spells[uti::SpellID::WHIRLWIND] = new Whirlwind(renderer);
    img[uti::SpellID::WHIRLWIND]    = spells[uti::SpellID::WHIRLWIND]->getImg();
    text[uti::SpellID::WHIRLWIND]   = spells[uti::SpellID::WHIRLWIND]->getText();

    spells[uti::SpellID::PUSH] = new Push(renderer);
    img[uti::SpellID::PUSH] = spells[uti::SpellID::PUSH]->getImg();
    text[uti::SpellID::PUSH] = spells[uti::SpellID::PUSH]->getText();

    updateBoxes();
}

Entity::~Entity()
{
    for (float i = 0; i < 4; i+=0.5)
    {
        SDL_FreeSurface(img[0][i][0]);
        SDL_DestroyTexture(text[0][i][0]);
        for (int j = 0; j < IMG_SIZE; j++)
        {
            SDL_FreeSurface(img[1][i][j]);
            SDL_DestroyTexture(text[1][i][j]);
        }
    }

    pseudo.clear();
    std::cout << "Entity: " << pseudo.getFont().getText() << " cleared !" << std::endl;
}

void Entity::move(vector<Element*>& v_elements, vector<Element*>& v_elements_solid, Map& m, bool& cameraLock, float& deltaTime, bool& sendSpellData, vector<SpellEffect>& spellEffects, chrono::high_resolution_clock::time_point now)
{
    //--- idle animation ---//
    if (!moving && !spellUsed) 
    { 

        if (step < 23 * animationSpeed)  step++;
        else                             step = 0;
        return; 
    }

    xChange = speed * xRate * deltaTime,
    yChange = speed * yRate * deltaTime;

    if (spellUsed)
    {
        xChange *= spellUsed->getBoostSpeed();
        yChange *= spellUsed->getBoostSpeed();
    }
    
    bool collision = false;

    for (Element* b : v_elements_solid) if (b->check_collisions(xMovebox + 5 * xChange, yMovebox + 5 * yChange)) { collision = true; break; }
    if (!collision)
    {
        updateMapPos(xChange, yChange);//pq ne pas juste changer l'offset si camera pas lock et changer le X si caméra lock ?? ça evite un double parcourt des maps pour le resetpos
        //cout << xMap << " : " << yMap << endl;
        for (unsigned int i = 0; i < v_elements.size(); i++)
        {
            v_elements[i]->addXOffset(-xChange);
            v_elements[i]->addYOffset(-yChange);
        }

        m.addOffset(-xChange, -yChange);

        if (!cameraLock)
        {
            pos.x = x += xChange;
            xOffset -= xChange;//-= car on veut revenir en arrière en ajoutant la valeur
            pos.y = y += yChange;
            yOffset -= yChange;//-= car on veut revenir en arrière en ajoutant la valeur
        }
        else
        {
            //for (Element* e : v_elements) e->resetPos(-xChange, -yChange);//applique le offset aux elements du décors
            for (Element* e : v_elements) e->resetPos();
            m.resetPos();
        }
    }

    updateBoxes();


    if (spellUsed && spellUsed->isAoe())
    {
        for (Element* e : v_elements)
        {
            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->getFaction() != faction)
            {
                if (spellUsed->isInRange(centerBox, dynamic_cast<Entity*>(e)->getCenterBox()))
                {
                    bool entityFound = false;
                    for (int i = spellEffects.size() - 1; i >= 0; i--)
                    {
                        if (spellEffects[i].entityID == dynamic_cast<Entity*>(e)->getID() && spellEffects[i].spellID == spellUsed->getID())
                        {
                            if (chrono::duration_cast<std::chrono::milliseconds>(now - spellEffects[i].last_hit_time).count() >= spellUsed->getHitSpeed())//si des dmg peuvent de nouveau être fait
                            {
                                cout << "Entity: " << dynamic_cast<Entity*>(e)->getID() << " in range !" << endl;
                                cout << chrono::duration_cast<std::chrono::milliseconds>(now - spellEffects[i].last_hit_time).count() << endl;
                                spellEffects.push_back({ dynamic_cast<Entity*>(e)->getID(), spellUsed->getID(), now});
                            }
                            entityFound = true;
                            break;
                        }
                    }

                    if (!entityFound)//Si on a pas trouvé l'entité c'est qu'elle n'a pas été touchée récemment
                    {
                        cout << "Entity: " << dynamic_cast<Entity*>(e)->getID() << " in range !" << endl;
                        spellEffects.push_back({ dynamic_cast<Entity*>(e)->getID(), spellUsed->getID(), now});
                    }
                }
            }
        }
    }

    //cout << ((spellUsed) ? spellUsed->getID() : 0) << endl;

    if (!spellUsed)
    {
        if (dir == 0)
            if (step < 9 * animationSpeed)  step++;
            else                            step = 0;
        else if (dir == 1)
            if (step < 11 * animationSpeed) step++;
            else                            step = 0;
        else if (dir == 2)
            if (step < 8 * animationSpeed)  step++;
            else                            step = 0;
        else if (dir == 3)
            if (step < 11 * animationSpeed) step++;
            else                            step = 0;
        else if (dir == 0.5)
            if (step < 11 * animationSpeed) step++;
            else                            step = 0;
        else if (dir == 1.5)
            if (step < 11 * animationSpeed) step++;
            else                            step = 0;
        else if (dir == 2.5)
            if (step < 11 * animationSpeed) step++;
            else                            step = 0;
        else if (dir == 3.5)
            if (step < 11 * animationSpeed) step++;
            else                            step = 0;
    }
    else
    {
        if(spellUsed) spellUsed->run(*this);
        if (!spellUsed) sendSpellData = true;//si pointeur pas alloué alors on vient de le clear donc spell terminé
    }

    update();
}

bool Entity::inClickBox(int x, int y)
{
    return x > clickBox.x && x < clickBox.x + clickBox.w && y > clickBox.y && y < clickBox.y + clickBox.h;
}

bool Entity::isInGoodDirection()
{
    short xDiff = centerBox.center.x - target->getCenterBox().center.x, // > 0 target à gauche, < 0 target à droite
          yDiff = centerBox.center.y - target->getCenterBox().center.y;// > 0 target en haut  , < 0 target en bas


    if (xDiff >= 0 && yDiff >= 0)//quart supérieur gauche
    {
        if (dir == 0 || dir == 3.5 || dir == 3.0)
        {
            if      (xDiff <= 200) dir = 0;
            else if (xDiff >= 500) dir = 3.0;
            else                   dir = 3.5;

            return true;
        }
        return false;
    }
    else if (xDiff <= 0 && yDiff >= 0)//quart supérieur droit
    {
        if (dir == 0 || dir == 0.5 || dir == 1.0)
        {
            if      (xDiff >= -200) dir = 0;
            else if (xDiff <= -500) dir = 1.0;
            else                    dir = 0.5;
            return true;
        }
        return false;
    }
    else if (xDiff <= 0 && yDiff <= 0)//quart inférieur droit
    {
        if (dir == 1.0 || dir == 1.5 || dir == 2)
        {
            if      (xDiff >= -200) dir = 2.0;
            else if (xDiff <= -200) dir = 1.0;
            else                    dir = 1.5;
            return true;
        }
        return false;
    }
    else if (xDiff >= 0 && yDiff <= 0)//quart inférieur gauche
    {
        if (dir == 2 || dir == 2.5 || dir == 3.0)
        {
            if      (xDiff <= 200) dir = 2.0;
            else if (xDiff >= 500) dir = .0;
            else                   dir = 2.5;
            return true;
        }

        return false;
    }
    return false;
}

void Entity::setHealth(int health)
{ 
    this->health = health;	
    posHealth.w = health;	
    if (health <= 0) 
    { 
        health = 0; 
        alive = false;
    } 
    else if (health > 100)
    {
        health = 100;
    }
}

void Entity::takeDamages(short dmg)
{
    this->health -= dmg;     
    posHealth.w = health;	
    if (health <= 0) 
    { 
        health = 0; 
        alive = false; 
    }
    else if (health > 100)
    {
        health = 100;
    }
}

void Entity::updateRBars()
{
    if (this->y > -50)
    {
        posBarH.y   = this->y + 40;
        posHealth.y = this->y + 40;
    }
    else
    {
        posBarH.y = this->pos.y + 200;
        posHealth.y = this->pos.y + 200;
    }

    posBarH.x   = this->x + 75;
    posHealth.x = this->x + 75;
}

void Entity::update()
{
    bool inAction = spellUsed && spellUsed->isMoving();
    //if (id == 1) if (spellUsed) cout << "ISMOVING: " << spellUsed->isMoving() << endl;
    
    switch (countDir)
    {
        case 0 : xRate =    0; yRate =    0;            if(spellUsed) break; moving = false; /*step = 0;*/ break;
        case 1 : xRate =    0; yRate =   -1; dir =   0; if(spellUsed) break; moving = true;                break;
        case 3 : xRate =    1; yRate =    0; dir =   1; if(spellUsed) break; moving = true;                break;
        case 6 : xRate =    0; yRate =    1; dir =   2; if(spellUsed) break; moving = true;                break;
        case 11: xRate =   -1; yRate =    0; dir =   3; if(spellUsed) break; moving = true;                break;
        case 4 : xRate =  0.5; yRate = -0.5; dir = 0.5; if(spellUsed) break; moving = true;                break;
        case 7 : xRate =    0; yRate =    0;            if(spellUsed) break; moving = false; /*step = 0;*/ break;
        case 12: xRate = -0.5; yRate = -0.5; dir = 3.5; if(spellUsed) break; moving = true;                break;
        case 9 : xRate =  0.5; yRate =  0.5; dir = 1.5; if(spellUsed) break; moving = true;                break;
        case 14: xRate =    0; yRate =    0;            if(spellUsed) break; moving = false; /*step = 0;*/ break;
        case 17: xRate = -0.5; yRate =  0.5; dir = 2.5; if(spellUsed) break; moving = true;                break;
        case 10: xRate =    1; yRate =    0; dir =   1; if(spellUsed) break; moving = true;                break;
        case 15: xRate =    0; yRate =   -1; dir =   0; if(spellUsed) break; moving = true;                break;
        case 18: xRate =   -1; yRate =    0; dir =   3; if(spellUsed) break; moving = true;                break;
        case 20: xRate =    0; yRate =    1; dir =   2; if(spellUsed) break; moving = true;                break;
        case 21: xRate =    0; yRate =    0;            if(spellUsed) break; moving = false; /*step = 0;*/ break;
        //default:
    }

    /*if (inAction)
    {
        xRate = uti::pixDir[dir].xRate;
        yRate = uti::pixDir[dir].yRate;
    }*/

    if (spellUsed) spellUsed->update(*this);

    if      (!spellUsed && moving == false && animationID != 0) animationID = 0;
    else if (!spellUsed && moving == true && animationID  != 1) animationID = 1;

    //cout << spellActive << " ANIMATIONID: " << animationID << endl;
}

void Entity::setSpell(short spellID)
{
    if (spellID == 0) 
    { 
        if (this->spellUsed) this->spellUsed->resetSpell(*this); 
        this->spellUsed   = nullptr; 
        this->animationID = 1;
        this->step        = 0;
        return; 
    } 

    this->spellUsed = spells[spellID];
}

void Entity::setHealthImg(short playerFaction, SDL_Renderer* renderer)
{
    SDL_Surface* imgHealth = IMG_Load(string(string("img/ui/ressources/") + string((faction == 0) ? "n_" : (faction == playerFaction) ? "a_" : "e_") + string("health.png")).c_str());
    if (imgHealth) textHealth = SDL_CreateTextureFromSurface(renderer, imgHealth);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> health.png");   exit(0); }
}

void Entity::setPos(float xMap, float yMap)
{
    this->xMap = xMap;
    this->yMap = yMap;
}

bool Entity::isInFront(int x, int y)
{
    return this->yMovebox > y;
}

bool Entity::check_collisions(int x, int y)
{
    return false;
}
