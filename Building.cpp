#include "Building.h"
#include "uti.h"

Building::Building(float x, float y, int width, int height, SDL_Renderer* renderer, string src) : Element(x, y, width, height)
{
    img = IMG_Load(string("img/building/" + src + ".png").c_str());

    imgBg = IMG_Load(string("img/building/" + src + "_bg.png").c_str());
    if (img) text = SDL_CreateTextureFromSurface(renderer, img);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load image: " + src).c_str()); exit(0); }

    isSolid = true;
}

Building::~Building()
{
        SDL_FreeSurface(img);
        SDL_FreeSurface(imgBg);
        SDL_DestroyTexture(text);

        cout << "Building is cleared now !" << endl;
}

bool Building::isInFront(int x, int y) {

    if (x < this->pos.x || x > this->pos.x + imgBg->w - 1 || y < this->pos.y || y > this->pos.y + imgBg->h - 1) return false;// check table de vérité ?

    Uint32 pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y); // - pos.x / - pos.y pour ???
    Uint8 red, green, blue, alpha;
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
    if (red == 0 && green == 0 && blue == 255) return true;

    pixel_color = uti::get_pixel(imgBg, x - pos.x + 10, y - pos.y); // - pos.x / - pos.y pour ???
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
    if (red == 0 && green == 0 && blue == 255) return true;

    pixel_color = uti::get_pixel(imgBg, x - pos.x - 10, y - pos.y); // - pos.x / - pos.y pour ???
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
    if (red == 0 && green == 0 && blue == 255) return true;

    pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y + 10); // - pos.x / - pos.y pour ???
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
    if (red == 0 && green == 0 && blue == 255) return true;

    pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y - 10); // - pos.x / - pos.y pour ???
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
    if (red == 0 && green == 0 && blue == 255) return true;

    //printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, red, green, blue, alpha);

    return false;
}

bool Building::check_collisions(int x, int y)
{
    if (!isSolid) return false;
    
    if (x < this->pos.x || x > this->pos.x + imgBg->w - 1 || y < this->pos.y || y > this->pos.y + imgBg->h - 1) return false;// check table de vérité ?

    Uint32 pixel_color = uti::get_pixel(imgBg, x - pos.x, y - pos.y); // - pos.x / - pos.y pour ???
    Uint8 red, green, blue, alpha;  
    SDL_GetRGBA(pixel_color, imgBg->format, &red, &green, &blue, &alpha);
    if (red == 255 && green == 0 && blue == 0) return true;

    //printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d, A=%d\n", x, y, red, green, blue, alpha);

    return false;
}

void Building::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, text, NULL, &pos);
}