#include "Entity.h"
#include "Building.h"

const map<short, string> Entity::types = {
    {TYPE::Humanoid, "Humanoïde"},
    {TYPE::Beast, "Bête"},
    {TYPE::Drake, "Dragon"}
};

Entity::Entity(std::string name, float xMap, float yMap, int id, int category, string imgSrc, SDL_Renderer* renderer) : Element(xMap, yMap, 250, 250 )
{
    this->pseudo = Pseudo(name, renderer);

    this->id       = id;
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

    imgPortrait = IMG_Load(string("img/entity/" + imgSrc + "/picture/head.png").c_str());
    if (imgPortrait) textPortrait = SDL_CreateTextureFromSurface(renderer, imgPortrait);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> portrait.png");   exit(0); }

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

    spells[0] = new AutoAttack(renderer);
    img[AutoAttack::animationID]  = spells[0]->getImg();
    text[AutoAttack::animationID] = spells[0]->getText();

    spells[1] = new Whirlwind(renderer);
    img[Whirlwind::animationID]    = spells[1]->getImg();
    text[Whirlwind::animationID]   = spells[1]->getText();

    updateMovebox();
    updateClickBox();
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

void Entity::move(vector<Element*>& v_elements, vector<Element*>& v_elements_solid, Map& m, bool& cameraLock, float& deltaTime)
{
    xChange = speed * xRate * deltaTime,
    yChange = speed * yRate * deltaTime;
    
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

    updateMovebox();
    updateClickBox();


    if (dir == 0)
        if (step < 9 * ANIMATIONMULTIPL)  step++;
        else                                step = 0;
    else if (dir == 1)
        if (step < 11 * ANIMATIONMULTIPL) step++;
        else                                step = 0;
    else if (dir == 2)
        if (step < 8 * ANIMATIONMULTIPL)  step++;
        else                                step = 0;
    else if (dir == 3)
        if (step < 11 * ANIMATIONMULTIPL) step++;
        else                                step = 0;
    else if (dir == 0.5)
        if (step < 11 * ANIMATIONMULTIPL) step++;
        else                                step = 0;
    else if (dir == 1.5)
        if (step < 11 * ANIMATIONMULTIPL) step++;
        else                                step = 0;
    else if (dir == 2.5)
        if (step < 11 * ANIMATIONMULTIPL) step++;
        else                                 step = 0;
    else if (dir == 3.5)
        if (step < 11 * ANIMATIONMULTIPL) step++;
        else                                step = 0;

    if (!moving) { step = 0; this->xChange = 0; this->yChange = 0; }
}

bool Entity::inClickBox(int x, int y)
{
    return x > clickBox.x && x < clickBox.x + clickBox.w && y > clickBox.y && y < clickBox.y + clickBox.h;
}

void Entity::update()
{
    switch (countDir)
    {
        case 0 : xRate =    0; yRate =    0;            moving = false; step = 0; break;
        case 1 : xRate =    0; yRate =   -1; dir =   0; moving = true;            break;
        case 3 : xRate =    1; yRate =    0; dir =   1; moving = true;            break;
        case 6 : xRate =    0; yRate =    1; dir =   2; moving = true;            break;
        case 11: xRate =   -1; yRate =    0; dir =   3; moving = true;            break;
        case 4 : xRate =  0.5; yRate = -0.5; dir = 0.5; moving = true;            break;
        case 7 : xRate =    0; yRate =    0;            moving = false; step = 0; break;
        case 12: xRate = -0.5; yRate = -0.5; dir = 3.5; moving = true;            break;
        case 9 : xRate =  0.5; yRate =  0.5; dir = 1.5; moving = true;            break;
        case 14: xRate =    0; yRate =    0;            moving = false; step = 0; break;
        case 17: xRate = -0.5; yRate =  0.5; dir = 2.5; moving = true;            break;
        case 10: xRate =    1; yRate =    0; dir =   1; moving = true;            break;
        case 15: xRate =    0; yRate =   -1; dir =   0; moving = true;            break;
        case 18: xRate =   -1; yRate =    0; dir =   3; moving = true;            break;
        case 20: xRate =    0; yRate =    1; dir =   2; moving = true;            break;
        case 21: xRate =    0; yRate =    0;            moving = false; step = 0; break;
        //default:
    }

    if      (moving == false && animationID == 1) animationID = 0;
    else if (moving == true && animationID == 0)  animationID = 1;
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
