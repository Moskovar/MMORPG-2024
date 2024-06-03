#include "Entity.h"
#include "Building.h"

const map<short, string> Entity::types = {
    {TYPE::Humanoid, "Humanoïde"},
    {TYPE::Beast, "Bête"},
    {TYPE::Drake, "Dragon"}
};

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
    this->spellActive = false;
    this->aaActive    = false;
    this->cancelAA    = false;

    this->xRate = 0;
    this->yRate = 0;

    posBarH.x  = x;
    posBarH.y  = y;
    posBarH.w  = 100;
    posBarH.h  = 10;
    posHealth.h = 10;

    centerBox.radius = 25;

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

    spells[AutoAttack::id] = new AutoAttack(renderer);
    img[AutoAttack::animationID]  = spells[AutoAttack::id]->getImg();
    text[AutoAttack::animationID] = spells[AutoAttack::id]->getText();

    spells[Whirlwind::id] = new Whirlwind(renderer);
    img[Whirlwind::animationID]    = spells[Whirlwind::id]->getImg();
    text[Whirlwind::animationID]   = spells[Whirlwind::id]->getText();

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

void Entity::move(vector<Element*>& v_elements, vector<Element*>& v_elements_solid, Map& m, bool& cameraLock, float& deltaTime, bool& sendSpellData, bool& sendSpellEffectData, vector<Entity*>& entitiesDamaged)
{
    if (!moving && (!spellUsed || (spellUsed && !spellUsed->isMoving()))) return;

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


    if (spellUsed)
    {
        spellUsed->checkDmgDone();
        if (!spellUsed->isDmgDone())
        {
            for (Element* e : v_elements)
            {
                if (dynamic_cast<Entity*>(e))
                {
                    if (spellUsed->isInRange(centerBox, dynamic_cast<Entity*>(e)->getCenterBox()))
                    {
                        spellUsed->setDmgDone();
                        sendSpellEffectData = true;
                        entitiesDamaged.push_back(dynamic_cast<Entity*>(e));
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
            else                                step = 0;
        else if (dir == 1)
            if (step < 11 * animationSpeed) step++;
            else                                step = 0;
        else if (dir == 2)
            if (step < 8 * animationSpeed)  step++;
            else                                step = 0;
        else if (dir == 3)
            if (step < 11 * animationSpeed) step++;
            else                                step = 0;
        else if (dir == 0.5)
            if (step < 11 * animationSpeed) step++;
            else                                step = 0;
        else if (dir == 1.5)
            if (step < 11 * animationSpeed) step++;
            else                                step = 0;
        else if (dir == 2.5)
            if (step < 11 * animationSpeed) step++;
            else                                 step = 0;
        else if (dir == 3.5)
            if (step < 11 * animationSpeed) step++;
            else                                step = 0;
    }
    else
    {
        if(spellUsed) spellUsed->run(v_elements, v_elements_solid, *this, nullptr);
        if (!spellUsed) sendSpellData = true;//si pointeur pas alloué alors on vient de le clear donc spell terminé
    }

    update();
}

bool Entity::inClickBox(int x, int y)
{
    return x > clickBox.x && x < clickBox.x + clickBox.w && y > clickBox.y && y < clickBox.y + clickBox.h;
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
        case 0 : xRate =    0; yRate =    0;            if(inAction) break; moving = false; step = 0; break;
        case 1 : xRate =    0; yRate =   -1; dir =   0; if(inAction) break; moving = true;            break;
        case 3 : xRate =    1; yRate =    0; dir =   1; if(inAction) break; moving = true;            break;
        case 6 : xRate =    0; yRate =    1; dir =   2; if(inAction) break; moving = true;            break;
        case 11: xRate =   -1; yRate =    0; dir =   3; if(inAction) break; moving = true;            break;
        case 4 : xRate =  0.5; yRate = -0.5; dir = 0.5; if(inAction) break; moving = true;            break;
        case 7 : xRate =    0; yRate =    0;            if(inAction) break; moving = false; step = 0; break;
        case 12: xRate = -0.5; yRate = -0.5; dir = 3.5; if(inAction) break; moving = true;            break;
        case 9 : xRate =  0.5; yRate =  0.5; dir = 1.5; if(inAction) break; moving = true;            break;
        case 14: xRate =    0; yRate =    0;            if(inAction) break; moving = false; step = 0; break;
        case 17: xRate = -0.5; yRate =  0.5; dir = 2.5; if(inAction) break; moving = true;            break;
        case 10: xRate =    1; yRate =    0; dir =   1; if(inAction) break; moving = true;            break;
        case 15: xRate =    0; yRate =   -1; dir =   0; if(inAction) break; moving = true;            break;
        case 18: xRate =   -1; yRate =    0; dir =   3; if(inAction) break; moving = true;            break;
        case 20: xRate =    0; yRate =    1; dir =   2; if(inAction) break; moving = true;            break;
        case 21: xRate =    0; yRate =    0;            if(inAction) break; moving = false; step = 0; break;
        //default:
    }

    if (inAction)
    {
        xRate = uti::pixDir[dir].xRate;
        yRate = uti::pixDir[dir].yRate;
    }

    if      (!spellActive && moving == false && animationID != 0) animationID = 0;
    else if (!spellActive && moving == true && animationID  != 1) animationID = 1;

    //cout << spellActive << " ANIMATIONID: " << animationID << endl;
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
