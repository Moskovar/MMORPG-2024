#include "Font.h"

Font::Font(SDL_Window* window, SDL_Renderer* renderer, string text)
{
    this->text = text;

    //// Chargement de la police
    font = TTF_OpenFont("fonts/roboto/Roboto-Regular.ttf", 24);
    if (font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    //Création de la surface de texte
    SDL_Color color = { 255, 255, 255 }; // Blanc
    img = TTF_RenderText_Blended(font, text.c_str(), color);
    if (img == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }

    // Création de la texture de texte
    texture = SDL_CreateTextureFromSurface(renderer, img);
    if (texture == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_FreeSurface(img);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
}

void Font::clear()
{
    TTF_CloseFont(font);
    SDL_FreeSurface(img);
    SDL_DestroyTexture(texture);
}
