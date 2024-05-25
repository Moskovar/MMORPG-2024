#include "uti.h"

namespace uti {
    map<int, map<int, string>> categories = {
        {
            Language::FR,   {
                                { Category::PLAYER, "Personnage joueur"    },
                                { Category::NPC   , "Personnage non joueur"}
                            },
        },
        { 
            Language::ENG,  {
                                { Category::PLAYER, "Player character"       },
                                { Category::NPC   , "Non-playable character" }
                            }
        }
    };

    std::map<float, MoveRate> pixDir = {
    {0,   {0,  -1}},
    {1,   {1,   0}},
    {2,   {0,   1}},
    {3,  {-1,  0}},
    {0.5,   {0.5, -0.5}},
    {3.5,  {-0.5,-0.5}},
    {1.5,   {0.5,  0.5}},
    {2.5,  {-0.5, 0.5}},
    };
    uint64_t getCurrentTimestamp()
    {
        return static_cast<uint64_t>(std::time(nullptr));
    }

    Uint32 get_pixel(SDL_Surface* surface, int x, int y)
    {
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
            return 0; // Format non supporté
        }
    }
}