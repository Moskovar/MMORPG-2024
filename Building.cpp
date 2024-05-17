#include "Building.h"

Building::Building(float x, float y, int width, int height, SDL_Renderer* renderer, string src) : Element(x, y, width, height)
{
    img = IMG_Load(string("img/building/" + src + ".png").c_str());

    imgBg = IMG_Load(string("img/building/" + src + "_bg.png").c_str());
    if (img) text = SDL_CreateTextureFromSurface(renderer, img);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

}

Building::~Building()
{
        SDL_FreeSurface(img);
        SDL_FreeSurface(imgBg);
        SDL_DestroyTexture(text);

        cout << "Building is cleared now !" << endl;
}

Uint32 get_pixel(SDL_Surface* surface, int x, int y) {
    // Obtenir le format de pixel de la surface
    SDL_PixelFormat* format = surface->format;

    // Calculer l'offset du pixel
    int bpp = format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    // Lire la valeur du pixel selon le format de pixel
    switch (bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16*)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32*)p;
    default:
        return 0; // Format non support�
    }
}
#include <Windows.h>
bool Building::isInFront(int x, int y) {
    if (x < this->pos.x || x > this->pos.x + imgBg->w - 1 || y < this->pos.y || y > this->pos.y + imgBg->h - 1) return false;// check table de v�rit� ?

    Uint32 pixel_color = get_pixel(imgBg, x - pos.x, y - pos.y);

    // Obtenir les composantes de couleur RGBA
    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);


    //printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, red, green, blue, alpha);

    //printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, red, green, blue, alpha);
    if (red == 0 && green == 0 && blue == 255) return true;
    return false;
}

bool Building::check_collisions(int x, int y)
{
    if (x < this->pos.x || x > this->pos.x + imgBg->w - 1 || y < this->pos.y || y > this->pos.y + imgBg->h - 1) return false;// check table de v�rit� ?

    Uint32 pixel_color = get_pixel(imgBg, x - pos.x, y - pos.y);

    // Obtenir les composantes de couleur RGBA
    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);

    if (red == 255 && green == 0 && blue == 0) { printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, red, green, blue, alpha);  return true; }
    //printf("COLLISIONS -> Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d, wasinfront: %d\n", x, y, red, green, blue, alpha, wasInFront);

    return false;
}

void Building::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, text, NULL, &pos);
}

void Building::resetPos()
{
    x += xOffset;
    pos.x = x;
    y += yOffset;
    pos.y = y;
    xOffset = 0;
    yOffset = 0;
}
