#include "Font.h"

Font::Font(string text, int size, short r, short g, short b, short a, SDL_Renderer* renderer)
{
    this->renderer = renderer;
    this->text = text;

    // Chargement de la police
    font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", size);
    if (font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        TTF_Quit();
    }

    //Création de la surface de texte
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    img = TTF_RenderText_Blended(font, text.c_str(), color);
    if (img == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
    }

    // Création de la texture de texte
    texture = SDL_CreateTextureFromSurface(renderer, img);
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_FreeSurface(img);
        TTF_CloseFont(font);
        TTF_Quit();
    }

    this->pos.w = img->w;
    this->pos.h = img->h;
}

void Font::clear()
{
    TTF_CloseFont(font);
    SDL_FreeSurface(img);
    SDL_DestroyTexture(texture);
}

void Font::draw(SDL_Renderer* renderer, int x, int y)
{
    this->pos.x = x;
    this->pos.y = y;
    SDL_RenderCopy(renderer, texture, NULL, &this->pos);
}
