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
}