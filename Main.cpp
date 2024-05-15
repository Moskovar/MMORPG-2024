#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <map>
#include <Windows.h>
#include <thread>
#include <vector>

#include "Warrior.h"
#include "Building.h"
#include "QuestBook.h"
#include "Font.h"

using namespace std;

SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };

SDL_bool run = SDL_TRUE;
Uint32 flags;
float deltaTime = 0;

vector<Element*> v_entities;
vector<Element*> v_decorations;
vector<vector<Element*>*> v_elements = { &v_entities, &v_decorations };


float posi_cameraSpeed = 5, nega_cameraSpeed = -posi_cameraSpeed;
bool cameraLock = false;
int mouseX = -1, mouseY = -1;

void t_move_player(Entity* e)
{
    while (e->isAlive())
    {
        if (e->isMoving() && !e->isSpellActive()) e->move(v_elements[1], cameraLock);
        Sleep(1);
    }
}

void t_run_spell(Spell* spell, Entity* e)
{
    spell->run(*v_elements[1], *e, cameraLock);
    if (e->getCountDir() == 0) e->setMoving(false);
}

void t_run_aa(AutoAttack* spell, Entity* e)
{
    while (!e->getCancelAA())
    {
        spell->run(*v_elements[1], *e, cameraLock);
        if (e->getCountDir() == 0) e->setMoving(false);
        for (int p = 0; p < 1000; p++)//METTRE UN DELTATIME POUR QUE LA PAUSE SOIT SAME POUR TLM
        {
            if (e->getCancelAA()) break;
            Sleep(1);
        }
    }
    e->setAAActive(false);
}

void t_update_camera()//revoir les paramètres des threads alors que variables globales
{
    while (run)
    {
        flags = SDL_GetWindowFlags(window);
        if(flags & SDL_WINDOW_INPUT_FOCUS)
            if (!cameraLock)
            {
                if (mouseX == 0) { for (vector<Element*>* v : v_elements) for (Element* e : *v) e->updateXOffset(posi_cameraSpeed); }//on déplace la caméra dans un sens et on enregistre l'offset dans l'autre sens pour revenir au point de départ
                else if (mouseX >= 1919) { for (vector<Element*>* v : v_elements) for (Element* e : *v) e->updateXOffset(nega_cameraSpeed); }
                if (mouseY == 0) { for (vector<Element*>* v : v_elements) for (Element* e : *v) e->updateYOffset(posi_cameraSpeed); }
                else if (mouseY >= 1079) { for (vector<Element*>* v : v_elements) for (Element* e : *v) e->updateYOffset(nega_cameraSpeed); }
            }
            else for(Element* e : *(v_elements)[1]) e->resetPos();
        Sleep(1);
    }
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(1024, 768, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS, &window, &renderer) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_Surface* loading_screen_img = IMG_Load(std::string("img/loadingscreen/1.jpg").c_str());
    SDL_Texture* loading_screen_texture = nullptr;

    SDL_Rect loading_screen_pos = { 0, 0, 1024, 768 };

    if (loading_screen_img) loading_screen_texture = SDL_CreateTextureFromSurface(renderer, loading_screen_img);
    else { SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, std::string("Failed to load loading screen").c_str());   exit(0); }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, loading_screen_texture, NULL, &loading_screen_pos);
    SDL_RenderPresent(renderer);
    
    SDL_Event events;
    
    Warrior w("Titus", uti::Category::PLAYER, window, renderer);
    Building b(0, 0, 1000, 672, renderer, "tavern");
    QuestBook qb(850, 200, renderer);

    v_elements[0]->push_back(dynamic_cast<Element*>(&w));
    v_elements[1]->push_back(dynamic_cast<Element*>(&b));

    // Variables pour le calcul du delta time
    thread t_player(t_move_player, &w);
    thread t_spell;
    thread t_aa;
    thread t_camera(t_update_camera);
    SDL_RenderClear(renderer);
    SDL_SetWindowSize(window, 1920, 1080);
    SDL_SetWindowPosition(window, 0, 0);

    // Activer la capture de la souris dans la fenêtre
    SDL_SetWindowGrab(window, SDL_TRUE);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);    

    while (run) 
    {
        while (SDL_PollEvent(&events)) 
        {
            switch (events.type)
            {
                case SDL_WINDOWEVENT: if (events.window.event == SDL_WINDOWEVENT_CLOSE) run = SDL_FALSE; break;
                case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué
                    if (events.key.keysym.sym == SDLK_z) { if (!w.up)    w.countDir += uti::Direction::UP;    w.up = true;    w.update(); }
                    if (events.key.keysym.sym == SDLK_d) { if (!w.right) w.countDir += uti::Direction::RIGHT; w.right = true; w.update(); }
                    if (events.key.keysym.sym == SDLK_s) { if (!w.down)  w.countDir += uti::Direction::DOWN;  w.down = true;  w.update(); }
                    if (events.key.keysym.sym == SDLK_q) { if (!w.left)  w.countDir += uti::Direction::LEFT;  w.left = true;  w.update(); }
                    if (events.key.keysym.sym == SDLK_a) { if (!w.isSpellActive()) { w.setCancelAA(true); if (!w.isSpellActive()) { if (t_spell.joinable()) t_spell.join(); t_spell = thread(t_run_spell, w.getSpell(1), &w); } } }
                    if (events.key.keysym.sym == SDLK_e) { if (!w.isSpellActive()) { if (!w.isAAActive()) { if (t_aa.joinable()) t_aa.join(); t_aa = thread(t_run_aa, dynamic_cast<AutoAttack*>(w.getSpell(0)), &w); } } }
                    if (events.key.keysym.sym == SDLK_SPACE) { for(vector<Element*>* v : v_elements) for(Element* e : *v) e->resetPos(); cameraLock = true; } 
                    if (events.key.keysym.sym == SDLK_y) { cameraLock = !cameraLock; for (vector<Element*>* v : v_elements) for (Element* e : *v) e->resetPos(); }
                    break;
                case SDL_KEYUP: // Un événement de type touche relâchée est effectué                
                    if (events.key.keysym.sym == SDLK_ESCAPE) { run = SDL_FALSE; w.setAlive(false); w.setCancelAA(true); }
                    if (events.key.keysym.sym == SDLK_z) { if (w.up) w.countDir -= uti::Direction::UP;     w.up = false;    w.update(); }
                    if (events.key.keysym.sym == SDLK_d) { if (w.right) w.countDir -= uti::Direction::RIGHT;  w.right = false; w.update(); }
                    if (events.key.keysym.sym == SDLK_q) { if (w.left) w.countDir -= uti::Direction::LEFT;   w.left = false;  w.update(); }
                    if (events.key.keysym.sym == SDLK_s) { if (w.down) w.countDir -= uti::Direction::DOWN;   w.down = false;  w.update(); }
                    if (events.key.keysym.sym == SDLK_SPACE) cameraLock = false;

                    //--- UI SHORTCUTS ---//
                    if (events.key.keysym.sym == SDLK_l) qb.setToDraw(!qb.isToDraw());
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = events.motion.x;
                    mouseY = events.motion.y;
                    break;
            }
        }

        SDL_RenderClear(renderer);
        
        b.draw(renderer);
        w.draw(renderer);

        //--- DRAW UI ---//
        if(qb.isToDraw()) qb.draw(renderer);
        //--------------//
        SDL_RenderPresent(renderer);
        Sleep(1);
    }

    if(t_player.joinable())  t_player.join();
    if(t_spell.joinable())   t_spell.join();
    if(t_aa.joinable())      t_aa.join();
    if (t_camera.joinable()) t_camera.join();

    SDL_FreeSurface(loading_screen_img);
    SDL_DestroyTexture(loading_screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}