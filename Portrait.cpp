#include "Portrait.h"
#include <SDL_image.h>

Portrait::Portrait(int x, int y, SDL_Renderer* renderer, SDL_Texture* textEntity) : UIElement(x, y, 328, 124)
{
    img = IMG_Load("img/ui/portrait.png");

    if (img) text = SDL_CreateTextureFromSurface(renderer, img);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: portrait.png" ).c_str()); exit(0); }

    this->textEntity = textEntity;
}

void Portrait::draw(SDL_Renderer* renderer)
{
    if (textEntity != nullptr)
    {
        SDL_RenderCopy(renderer, text, NULL, &pos);
        SDL_RenderCopy(renderer, textEntity, NULL, &pos);
    }
}


