#include "Building.h"

Building::Building(float x, float y, int width, int height, SDL_Renderer* renderer, string src) : Element(x, y, width, height)
{
    img = IMG_Load(string("img/building/" + src + ".png").c_str());

    if (img) text = SDL_CreateTextureFromSurface(renderer, img);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

}

void Building::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, text, NULL, &pos);
}
