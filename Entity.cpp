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

    auto start = std::chrono::high_resolution_clock::now();

    //--- back-right ---//
    
    //--- idle ---//
    imgHair[0][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/idle/hair.png").c_str());
    if (imgHair[0][0.5]) textHair[0][0.5] = SDL_CreateTextureFromSurface(renderer, imgHair[0][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right hair.png"); exit(0); }

    //imgHead[0][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/idle/head.png").c_str());
    //if (imgHead[0][0.5]) textHead[0][0.5] = SDL_CreateTextureFromSurface(renderer, imgHead[0][0.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right head.png"); exit(0); }

    imgBody[0][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/idle/body.png").c_str());
    if (imgBody[0][0.5]) textBody[0][0.5] = SDL_CreateTextureFromSurface(renderer, imgBody[0][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right body.png"); exit(0); }

    imgArmL[0][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/idle/arml.png").c_str());
    if (imgArmL[0][0.5]) textArmL[0][0.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[0][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right arml.png"); exit(0); }

    imgArmR[0][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/idle/armr.png").c_str());
    if (imgArmR[0][0.5]) textArmR[0][0.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[0][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right armr.png"); exit(0); }

    imgFull[0][0.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[0][0.5]->w, imgBody[0][0.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[0][0.5], NULL, imgFull[0][0.5], NULL);
    SDL_BlitSurface(imgBody[0][0.5], NULL, imgFull[0][0.5], NULL);
    SDL_BlitSurface(imgArmR[0][0.5], NULL, imgFull[0][0.5], NULL);
    //SDL_BlitSurface(imgHead[0][0.5], NULL, imgFull[0][0.5], NULL);
    SDL_BlitSurface(imgHair[0][0.5], NULL, imgFull[0][0.5], NULL);

    if (imgFull[0][0.5]) textFull[0][0.5] = SDL_CreateTextureFromSurface(renderer, imgFull[0][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right full"); exit(0); }

    //--- run ---//

    imgHair[1][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/run/hair.png").c_str());
    if (imgHair[1][0.5]) textHair[1][0.5] = SDL_CreateTextureFromSurface(renderer, imgHair[1][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run back-right hair.png"); exit(0); }

    //imgHead[1][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/run/head.png").c_str());
    //if (imgHead[1][0.5]) textHead[1][0.5] = SDL_CreateTextureFromSurface(renderer, imgHead[1][0.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run back-right head.png"); exit(0); }

    imgBody[1][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/run/body.png").c_str());
    if (imgBody[1][0.5]) textBody[1][0.5] = SDL_CreateTextureFromSurface(renderer, imgBody[1][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run back-right body.png"); exit(0); }

    imgArmL[1][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/run/arml.png").c_str());
    if (imgArmL[1][0.5]) textArmL[1][0.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[1][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run back-right arml.png"); exit(0); }

    imgArmR[1][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/run/armr.png").c_str());
    if (imgArmR[1][0.5]) textArmR[1][0.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[1][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run back-right armr.png"); exit(0); }

    imgFull[1][0.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[1][0.5]->w, imgBody[1][0.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[1][0.5], NULL, imgFull[1][0.5], NULL);
    SDL_BlitSurface(imgBody[1][0.5], NULL, imgFull[1][0.5], NULL);
    SDL_BlitSurface(imgArmR[1][0.5], NULL, imgFull[1][0.5], NULL);
    //SDL_BlitSurface(imgHead[1][0.5], NULL, imgFull[1][0.5], NULL);
    SDL_BlitSurface(imgHair[1][0.5], NULL, imgFull[1][0.5], NULL);

    if (imgFull[1][0.5]) textFull[1][0.5] = SDL_CreateTextureFromSurface(renderer, imgFull[1][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right full"); exit(0); }

    //--- auto attack ---//

    imgHair[3][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/aa/hair.png").c_str());
    if (imgHair[3][0.5]) textHair[3][0.5] = SDL_CreateTextureFromSurface(renderer, imgHair[3][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa back-right hair.png"); exit(0); }

    //imgHead[3][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/aa/head.png").c_str());
    //if (imgHead[3][0.5]) textHead[3][0.5] = SDL_CreateTextureFromSurface(renderer, imgHead[3][0.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa back-right head.png"); exit(0); }

    imgBody[3][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/aa/body.png").c_str());
    if (imgBody[3][0.5]) textBody[3][0.5] = SDL_CreateTextureFromSurface(renderer, imgBody[3][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa back-right body.png"); exit(0); }

    imgArmL[3][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/aa/arml.png").c_str());
    if (imgArmL[3][0.5]) textArmL[3][0.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[3][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa back-right arml.png"); exit(0); }

    imgArmR[3][0.5] = IMG_Load(string("img/entity/" + imgSrc + "/back-right/aa/armr.png").c_str());
    if (imgArmR[3][0.5]) textArmR[3][0.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[3][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa back-right armr.png"); exit(0); }

    imgFull[3][0.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[3][0.5]->w, imgBody[3][0.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[3][0.5], NULL, imgFull[3][0.5], NULL);
    SDL_BlitSurface(imgBody[3][0.5], NULL, imgFull[3][0.5], NULL);
    SDL_BlitSurface(imgArmR[3][0.5], NULL, imgFull[3][0.5], NULL);
    //SDL_BlitSurface(imgHead[3][0.5], NULL, imgFull[3][0.5], NULL);
    SDL_BlitSurface(imgHair[3][0.5], NULL, imgFull[3][0.5], NULL);

    if (imgFull[3][0.5]) textFull[3][0.5] = SDL_CreateTextureFromSurface(renderer, imgFull[3][0.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle back-right full"); exit(0); }


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

    imgFull[0][1] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[0][1]->w, imgBody[0][1]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[0][1], NULL, imgFull[0][1], NULL);
    SDL_BlitSurface(imgBody[0][1], NULL, imgFull[0][1], NULL);
    SDL_BlitSurface(imgArmR[0][1], NULL, imgFull[0][1], NULL);
    SDL_BlitSurface(imgHead[0][1], NULL, imgFull[0][1], NULL);
    SDL_BlitSurface(imgHair[0][1], NULL, imgFull[0][1], NULL);

    if (imgFull[0][1]) textFull[0][1] = SDL_CreateTextureFromSurface(renderer, imgFull[0][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right full"); exit(0); }

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

    imgFull[1][1] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[1][1]->w, imgBody[1][1]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[1][1], NULL, imgFull[1][1], NULL);
    SDL_BlitSurface(imgBody[1][1], NULL, imgFull[1][1], NULL);
    SDL_BlitSurface(imgArmR[1][1], NULL, imgFull[1][1], NULL);
    SDL_BlitSurface(imgHead[1][1], NULL, imgFull[1][1], NULL);
    SDL_BlitSurface(imgHair[1][1], NULL, imgFull[1][1], NULL);

    if (imgFull[1][1]) textFull[1][1] = SDL_CreateTextureFromSurface(renderer, imgFull[1][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right full"); exit(0); }


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

    imgFull[3][1] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[3][1]->w, imgBody[3][1]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[3][1], NULL, imgFull[3][1], NULL);
    SDL_BlitSurface(imgBody[3][1], NULL, imgFull[3][1], NULL);
    SDL_BlitSurface(imgArmR[3][1], NULL, imgFull[3][1], NULL);
    SDL_BlitSurface(imgHead[3][1], NULL, imgFull[3][1], NULL);
    SDL_BlitSurface(imgHair[3][1], NULL, imgFull[3][1], NULL);

    if (imgFull[3][1]) textFull[3][1] = SDL_CreateTextureFromSurface(renderer, imgFull[3][1]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle right full"); exit(0); }


    //--- front-right ---//

    //--- idle ---//
    imgHair[0][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/idle/hair.png").c_str());
    if (imgHair[0][1.5]) textHair[0][1.5] = SDL_CreateTextureFromSurface(renderer, imgHair[0][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right hair.png"); exit(0); }

    //imgHead[0][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/idle/head.png").c_str());
    //if (imgHead[0][1.5]) textHead[0][1.5] = SDL_CreateTextureFromSurface(renderer, imgHead[0][1.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right head.png"); exit(0); }

    imgBody[0][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/idle/body.png").c_str());
    if (imgBody[0][1.5]) textBody[0][1.5] = SDL_CreateTextureFromSurface(renderer, imgBody[0][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right body.png"); exit(0); }

    imgArmL[0][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/idle/arml.png").c_str());
    if (imgArmL[0][1.5]) textArmL[0][1.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[0][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right arml.png"); exit(0); }

    imgArmR[0][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/idle/armr.png").c_str());
    if (imgArmR[0][1.5]) textArmR[0][1.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[0][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right armr.png"); exit(0); }

    imgFull[0][1.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[0][1.5]->w, imgBody[0][1.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[0][1.5], NULL, imgFull[0][1.5], NULL);
    SDL_BlitSurface(imgBody[0][1.5], NULL, imgFull[0][1.5], NULL);
    SDL_BlitSurface(imgArmR[0][1.5], NULL, imgFull[0][1.5], NULL);
    //SDL_BlitSurface(imgHead[0][1.5], NULL, imgFull[0][1.5], NULL);
    SDL_BlitSurface(imgHair[0][1.5], NULL, imgFull[0][1.5], NULL);

    if (imgFull[0][1.5]) textFull[0][1.5] = SDL_CreateTextureFromSurface(renderer, imgFull[0][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right full"); exit(0); }

    //--- run ---//

    imgHair[1][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/run/hair.png").c_str());
    if (imgHair[1][1.5]) textHair[1][1.5] = SDL_CreateTextureFromSurface(renderer, imgHair[1][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-right hair.png"); exit(0); }

    //imgHead[1][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/run/head.png").c_str());
    //if (imgHead[1][1.5]) textHead[1][1.5] = SDL_CreateTextureFromSurface(renderer, imgHead[1][1.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-right head.png"); exit(0); }

    imgBody[1][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/run/body.png").c_str());
    if (imgBody[1][1.5]) textBody[1][1.5] = SDL_CreateTextureFromSurface(renderer, imgBody[1][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-right body.png"); exit(0); }

    imgArmL[1][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/run/arml.png").c_str());
    if (imgArmL[1][1.5]) textArmL[1][1.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[1][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-right arml.png"); exit(0); }

    imgArmR[1][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/run/armr.png").c_str());
    if (imgArmR[1][1.5]) textArmR[1][1.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[1][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-right armr.png"); exit(0); }

    imgFull[1][1.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[1][1.5]->w, imgBody[1][1.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[1][1.5], NULL, imgFull[1][1.5], NULL);
    SDL_BlitSurface(imgBody[1][1.5], NULL, imgFull[1][1.5], NULL);
    SDL_BlitSurface(imgArmR[1][1.5], NULL, imgFull[1][1.5], NULL);
    //SDL_BlitSurface(imgHead[1][1.5], NULL, imgFull[1][1.5], NULL);
    SDL_BlitSurface(imgHair[1][1.5], NULL, imgFull[1][1.5], NULL);

    if (imgFull[1][1.5]) textFull[1][1.5] = SDL_CreateTextureFromSurface(renderer, imgFull[1][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right full"); exit(0); }


    //--- auto attack ---//

    imgHair[3][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/aa/hair.png").c_str());
    if (imgHair[3][1.5]) textHair[3][1.5] = SDL_CreateTextureFromSurface(renderer, imgHair[3][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-right hair.png"); exit(0); }

    //imgHead[3][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/aa/head.png").c_str());
    //if (imgHead[3][1.5]) textHead[3][1.5] = SDL_CreateTextureFromSurface(renderer, imgHead[3][1.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-right head.png"); exit(0); }

    imgBody[3][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/aa/body.png").c_str());
    if (imgBody[3][1.5]) textBody[3][1.5] = SDL_CreateTextureFromSurface(renderer, imgBody[3][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-right body.png"); exit(0); }

    imgArmL[3][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/aa/arml.png").c_str());
    if (imgArmL[3][1.5]) textArmL[3][1.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[3][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-right arml.png"); exit(0); }

    imgArmR[3][1.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-right/aa/armr.png").c_str());
    if (imgArmR[3][1.5]) textArmR[3][1.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[3][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-right armr.png"); exit(0); }

    imgFull[3][1.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[3][1.5]->w, imgBody[3][1.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmL[3][1.5], NULL, imgFull[3][1.5], NULL);
    SDL_BlitSurface(imgBody[3][1.5], NULL, imgFull[3][1.5], NULL);
    SDL_BlitSurface(imgArmR[3][1.5], NULL, imgFull[3][1.5], NULL);
    //SDL_BlitSurface(imgHead[3][1.5], NULL, imgFull[3][1.5], NULL);
    SDL_BlitSurface(imgHair[3][1.5], NULL, imgFull[3][1.5], NULL);

    if (imgFull[3][1.5]) textFull[3][1.5] = SDL_CreateTextureFromSurface(renderer, imgFull[3][1.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-right full"); exit(0); }




    //--- front-left ---//

    //--- idle ---//
    imgHair[0][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/idle/hair.png").c_str());
    if (imgHair[0][2.5]) textHair[0][2.5] = SDL_CreateTextureFromSurface(renderer, imgHair[0][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left hair.png"); exit(0); }

    //imgHead[0][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/idle/head.png").c_str());
    //if (imgHead[0][2.5]) textHead[0][2.5] = SDL_CreateTextureFromSurface(renderer, imgHead[0][2.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left head.png"); exit(0); }

    imgBody[0][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/idle/body.png").c_str());
    if (imgBody[0][2.5]) textBody[0][2.5] = SDL_CreateTextureFromSurface(renderer, imgBody[0][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left body.png"); exit(0); }

    imgArmL[0][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/idle/arml.png").c_str());
    if (imgArmL[0][2.5]) textArmL[0][2.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[0][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left arml.png"); exit(0); }

    imgArmR[0][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/idle/armr.png").c_str());
    if (imgArmR[0][2.5]) textArmR[0][2.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[0][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left armr.png"); exit(0); }

    imgFull[0][2.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[0][2.5]->w, imgBody[0][2.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmR[0][2.5], NULL, imgFull[0][2.5], NULL);
    SDL_BlitSurface(imgBody[0][2.5], NULL, imgFull[0][2.5], NULL);
    SDL_BlitSurface(imgArmL[0][2.5], NULL, imgFull[0][2.5], NULL);
    //SDL_BlitSurface(imgHead[0][2.5], NULL, imgFull[0][2.5], NULL);
    SDL_BlitSurface(imgHair[0][2.5], NULL, imgFull[0][2.5], NULL);

    if (imgFull[0][2.5]) textFull[0][2.5] = SDL_CreateTextureFromSurface(renderer, imgFull[0][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left full"); exit(0); }

    //--- run ---//

    imgHair[1][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/run/hair.png").c_str());
    if (imgHair[1][2.5]) textHair[1][2.5] = SDL_CreateTextureFromSurface(renderer, imgHair[1][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-left hair.png"); exit(0); }

    //imgHead[1][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/run/head.png").c_str());
    //if (imgHead[1][2.5]) textHead[1][2.5] = SDL_CreateTextureFromSurface(renderer, imgHead[1][2.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-left head.png"); exit(0); }

    imgBody[1][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/run/body.png").c_str());
    if (imgBody[1][2.5]) textBody[1][2.5] = SDL_CreateTextureFromSurface(renderer, imgBody[1][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-left body.png"); exit(0); }

    imgArmL[1][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/run/arml.png").c_str());
    if (imgArmL[1][2.5]) textArmL[1][2.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[1][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-left arml.png"); exit(0); }

    imgArmR[1][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/run/armr.png").c_str());
    if (imgArmR[1][2.5]) textArmR[1][2.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[1][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run front-left armr.png"); exit(0); }

    imgFull[1][2.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[1][2.5]->w, imgBody[1][2.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmR[1][2.5], NULL, imgFull[1][2.5], NULL);
    SDL_BlitSurface(imgBody[1][2.5], NULL, imgFull[1][2.5], NULL);
    SDL_BlitSurface(imgArmL[1][2.5], NULL, imgFull[1][2.5], NULL);
    //SDL_BlitSurface(imgHead[1][2.5], NULL, imgFull[1][2.5], NULL);
    SDL_BlitSurface(imgHair[1][2.5], NULL, imgFull[1][2.5], NULL);

    if (imgFull[1][2.5]) textFull[1][2.5] = SDL_CreateTextureFromSurface(renderer, imgFull[1][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left full"); exit(0); }


    //--- auto attack ---//

    imgHair[3][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/aa/hair.png").c_str());
    if (imgHair[3][2.5]) textHair[3][2.5] = SDL_CreateTextureFromSurface(renderer, imgHair[3][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-left hair.png"); exit(0); }

    //imgHead[3][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/aa/head.png").c_str());
    //if (imgHead[3][2.5]) textHead[3][2.5] = SDL_CreateTextureFromSurface(renderer, imgHead[3][2.5]);
    //else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-left head.png"); exit(0); }

    imgBody[3][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/aa/body.png").c_str());
    if (imgBody[3][2.5]) textBody[3][2.5] = SDL_CreateTextureFromSurface(renderer, imgBody[3][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-left body.png"); exit(0); }

    imgArmL[3][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/aa/arml.png").c_str());
    if (imgArmL[3][2.5]) textArmL[3][2.5] = SDL_CreateTextureFromSurface(renderer, imgArmL[3][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-left arml.png"); exit(0); }

    imgArmR[3][2.5] = IMG_Load(string("img/entity/" + imgSrc + "/front-left/aa/armr.png").c_str());
    if (imgArmR[3][2.5]) textArmR[3][2.5] = SDL_CreateTextureFromSurface(renderer, imgArmR[3][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa front-left armr.png"); exit(0); }

    imgFull[3][2.5] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[3][2.5]->w, imgBody[3][2.5]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmR[3][2.5], NULL, imgFull[3][2.5], NULL);
    SDL_BlitSurface(imgBody[3][2.5], NULL, imgFull[3][2.5], NULL);
    SDL_BlitSurface(imgArmL[3][2.5], NULL, imgFull[3][2.5], NULL);
    //SDL_BlitSurface(imgHead[3][2.5], NULL, imgFull[3][2.5], NULL);
    SDL_BlitSurface(imgHair[3][2.5], NULL, imgFull[3][2.5], NULL);

    if (imgFull[3][2.5]) textFull[3][2.5] = SDL_CreateTextureFromSurface(renderer, imgFull[3][2.5]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle front-left full"); exit(0); }





    //--- Left ---//

    //--- idle ---//
    imgHair[0][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/idle/hair.png").c_str());
    if (imgHair[0][3]) textHair[0][3] = SDL_CreateTextureFromSurface(renderer, imgHair[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle left hair.png"); exit(0); }

    imgHead[0][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/idle/head.png").c_str());
    if (imgHead[0][3]) textHead[0][3] = SDL_CreateTextureFromSurface(renderer, imgHead[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle left head.png"); exit(0); }

    imgBody[0][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/idle/body.png").c_str());
    if (imgBody[0][3]) textBody[0][3] = SDL_CreateTextureFromSurface(renderer, imgBody[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle left body.pngpp"); exit(0); }

    imgArmL[0][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/idle/arml.png").c_str());
    if (imgArmL[0][3]) textArmL[0][3] = SDL_CreateTextureFromSurface(renderer, imgArmL[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle left arml.png"); exit(0); }

    imgArmR[0][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/idle/armr.png").c_str());
    if (imgArmR[0][3]) textArmR[0][3] = SDL_CreateTextureFromSurface(renderer, imgArmR[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle left armr.png"); exit(0); }//clear quand exit ??

    imgFull[0][3] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[0][3]->w, imgBody[0][3]->h, 32, SDL_PIXELFORMAT_RGBA32);

    

    SDL_BlitSurface(imgArmR[0][3], NULL, imgFull[0][3], NULL);
    SDL_BlitSurface(imgBody[0][3], NULL, imgFull[0][3], NULL);
    SDL_BlitSurface(imgArmL[0][3], NULL, imgFull[0][3], NULL);
    SDL_BlitSurface(imgHead[0][3], NULL, imgFull[0][3], NULL);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            SDL_Rect srcRect = { 500 * i, 500 * j, 500, 500 };
            SDL_Rect dstRect = { 500 * i, 500 * j, 500, 500 };
            SDL_BlitSurface(imgHair[0][3], &srcRect, imgFull[0][3], &dstRect);
        }
    }

    if (imgFull[0][3]) textFull[0][3] = SDL_CreateTextureFromSurface(renderer, imgFull[0][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle Left full"); exit(0); }

    //--- run ---//

    imgHair[1][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/run/hair.png").c_str());
    if (imgHair[1][3]) textHair[1][3] = SDL_CreateTextureFromSurface(renderer, imgHair[1][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run left hair.png"); exit(0); }

    imgHead[1][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/run/head.png").c_str());
    if (imgHead[1][3]) textHead[1][3] = SDL_CreateTextureFromSurface(renderer, imgHead[1][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run left head.png"); exit(0); }

    imgBody[1][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/run/body.png").c_str());
    if (imgBody[1][3]) textBody[1][3] = SDL_CreateTextureFromSurface(renderer, imgBody[1][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run left body.png"); exit(0); }

    imgArmL[1][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/run/arml.png").c_str());
    if (imgArmL[1][3]) textArmL[1][3] = SDL_CreateTextureFromSurface(renderer, imgArmL[1][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run left arml.png"); exit(0); }

    imgArmR[1][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/run/armr.png").c_str());
    if (imgArmR[1][3]) textArmR[1][3] = SDL_CreateTextureFromSurface(renderer, imgArmR[1][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> run left armr.png"); exit(0); }

    imgFull[1][3] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[1][3]->w, imgBody[1][3]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmR[1][3], NULL, imgFull[1][3], NULL);
    SDL_BlitSurface(imgBody[1][3], NULL, imgFull[1][3], NULL);
    SDL_BlitSurface(imgArmL[1][3], NULL, imgFull[1][3], NULL);
    SDL_BlitSurface(imgHead[1][3], NULL, imgFull[1][3], NULL);
    SDL_BlitSurface(imgHair[1][3], NULL, imgFull[1][3], NULL);

    if (imgFull[1][3]) textFull[1][3] = SDL_CreateTextureFromSurface(renderer, imgFull[1][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle Left full"); exit(0); }

    

    //--- auto attack ---//

    imgHair[3][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/aa/hair.png").c_str());
    if (imgHair[3][3]) textHair[3][3] = SDL_CreateTextureFromSurface(renderer, imgHair[3][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa left hair.png"); exit(0); }

    imgHead[3][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/aa/head.png").c_str());
    if (imgHead[3][3]) textHead[3][3] = SDL_CreateTextureFromSurface(renderer, imgHead[3][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa left head.png"); exit(0); }

    imgBody[3][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/aa/body.png").c_str());
    if (imgBody[3][3]) textBody[3][3] = SDL_CreateTextureFromSurface(renderer, imgBody[3][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa left body.png"); exit(0); }

    imgArmL[3][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/aa/arml.png").c_str());
    if (imgArmL[3][3]) textArmL[3][3] = SDL_CreateTextureFromSurface(renderer, imgArmL[3][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa left arml.png"); exit(0); }

    imgArmR[3][3] = IMG_Load(string("img/entity/" + imgSrc + "/left/aa/armr.png").c_str());
    if (imgArmR[3][3]) textArmR[3][3] = SDL_CreateTextureFromSurface(renderer, imgArmR[3][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> aa left armr.png"); exit(0); }

    imgFull[3][3] = SDL_CreateRGBSurfaceWithFormat(0, imgBody[3][3]->w, imgBody[3][3]->h, 32, SDL_PIXELFORMAT_RGBA32);

    SDL_BlitSurface(imgArmR[3][3], NULL, imgFull[3][3], NULL);
    SDL_BlitSurface(imgBody[3][3], NULL, imgFull[3][3], NULL);
    SDL_BlitSurface(imgArmL[3][3], NULL, imgFull[3][3], NULL);
    SDL_BlitSurface(imgHead[3][3], NULL, imgFull[3][3], NULL);
    SDL_BlitSurface(imgHair[3][3], NULL, imgFull[3][3], NULL);

    if (imgFull[3][3]) textFull[3][3] = SDL_CreateTextureFromSurface(renderer, imgFull[3][3]);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load image: Entity -> idle Left full"); exit(0); }


    // Obtenir le temps de fin
    auto end = std::chrono::high_resolution_clock::now();

    // Calculer la durée d'exécution
    std::chrono::duration<double> duration = end - start;

    // Afficher la durée
    std::cout << "La fonction a pris " << duration.count() << " secondes pour s'exécuter." << std::endl;




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

    spells[uti::SpellID::AA] = new AutoAttack(renderer);
    img[uti::SpellID::AA]    = spells[uti::SpellID::AA]->getImg();
    text[uti::SpellID::AA]   = spells[uti::SpellID::AA]->getText();

    updateBoxes();

    for (int i = 0; i < 100; i++)
    {
        for (float j = 0; j < 4; j += 0.5)
        {
            if (imgBody[i][j])//pour vérifier le i car on prend un nombre large d'animation ID, pas ouf
            {
                SDL_FreeSurface(imgHair[i][j]);
                SDL_DestroyTexture(textHair[i][j]);

                SDL_FreeSurface(imgHead[i][j]);
                SDL_DestroyTexture(textHead[i][j]);

                SDL_FreeSurface(imgBody[i][j]);
                SDL_DestroyTexture(textBody[i][j]);

                SDL_FreeSurface(imgArmR[i][j]);
                SDL_DestroyTexture(textArmR[i][j]);

                SDL_FreeSurface(imgArmL[i][j]);
                SDL_DestroyTexture(textArmL[i][j]);
            }
        }
    }
}

Entity::~Entity()
{
    for (float i = 0; i < 4; i+=0.5)
    {
        SDL_FreeSurface(img[0][i][0]);
        SDL_DestroyTexture(text[0][i][0]);
        for (int j = 0; j < IMG_SIZE; j++)
        {
            SDL_FreeSurface(img[0.5][i][j]);
            SDL_DestroyTexture(text[0.5][i][j]);
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
        if (step < 23 * uti::animationSpeeds[uti::SpellID::IDLE])  step++;
        else                                                       step = 0;
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
        if (step < 23 * animationSpeed) step++;
        else                            step = 0;
    }
    else
    {
        if(spellUsed)  spellUsed->run(*this);
        if (!spellUsed) sendSpellData = true;//si pointeur pas alloué alors on vient de le clear donc spell terminé
    }

    update();
}

short Entity::getStep()
{
    short animationSpeed = 0;
    if (!spellUsed)
    {
        if (xRate == 0 && yRate == 0) animationSpeed = uti::animationSpeeds[uti::SpellID::IDLE];
        else if (aaActive)                 animationSpeed = uti::animationSpeeds[uti::SpellID::AA];
        else if (xRate != 0 || yRate != 0) animationSpeed = uti::animationSpeeds[uti::SpellID::RUN];
    }
    else
    {
        if (aaActive) animationSpeed = uti::animationSpeeds[uti::SpellID::AA];
        else          animationSpeed = uti::animationSpeeds[spellUsed->getID()];
    }

    return this->step / animationSpeed;
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
