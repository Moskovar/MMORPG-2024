#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <map>
#include <Windows.h>
#include <thread>
#include <vector>
#include <algorithm>

#include "Warrior.h"
#include "NPC.h"
#include "Building.h"
#include "QuestBook.h"
#include "Portrait.h"
#include "Quest.h"
#include "Font.h"

using namespace std;

SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };

SDL_bool run = SDL_TRUE;
Uint32 flags;
float deltaTime = 0;

vector<vector<Element*>> v_elements = { {}, {} };

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
    spell->run(v_elements[1], *e, cameraLock);
    if (e->getCountDir() == 0) e->setMoving(false);
}

void t_run_aa(AutoAttack* spell, Entity* e)
{
    while (!e->getCancelAA())
    {
        spell->run(v_elements[1], *e, cameraLock);
        if (e->getCountDir() == 0) e->setMoving(false);
        for (int p = 0; p < 1000; p++)
        {
            if (e->getCancelAA()) break;
            Sleep(1);
        }
    }
    e->setAAActive(false);
}

void t_update_camera(Entity* e)
{
    while (run)//trouver un moyen de synchro avec le move du joueur
    {
        flags = SDL_GetWindowFlags(window);
        if(flags & SDL_WINDOW_INPUT_FOCUS)
            if (!cameraLock)
            {
                if (mouseX == 0) { for (vector<Element*> v : v_elements) for (Element* e : v) e->updateXOffset(posi_cameraSpeed); e->updateHitbox(); }//on déplace la caméra dans un sens et on enregistre l'offset dans l'autre sens pour revenir au point de départ
                else if (mouseX >= 1919) { for (vector<Element*> v : v_elements) for (Element* e : v) e->updateXOffset(nega_cameraSpeed); e->updateHitbox(); }
                if (mouseY == 0) { for (vector<Element*> v : v_elements) for (Element* e : v) e->updateYOffset(posi_cameraSpeed); e->updateHitbox(); }
                else if (mouseY >= 1079) { for (vector<Element*> v : v_elements) for (Element* e : v) e->updateYOffset(nega_cameraSpeed); e->updateHitbox(); }
            }
            //else for(Element* e : *(v_elements)[1]) e->resetPos();//applique le offset aux elements du décors
        Sleep(1);
    }
}

void draw_circle(SDL_Renderer* renderer, int center_x, int center_y, int radius) {
    int x = radius - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y) {
        // Dessiner les huit octants du cercle
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

bool compareZ(Element* e1, Element* e2)
{
    return e1->getY() < e2->getY();
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
    int posx = 1000, posy = 0;
    Warrior w("Titus", posx, posy, uti::Category::PLAYER, window, renderer);
    NPC npc("DENT", 900, 300, uti::Category::NPC, window, renderer, "character/warrior");

    Building b(0, 0, 1000, 672, renderer, "tavern/tavern");
    Building b2(250, 1500, 1000, 672, renderer, "tavern/tavern");
    Building b3(1000, 500, 1000, 672, renderer, "tavern/tavern");
    Building b4(1500, 800, 1000, 672, renderer, "tavern/tavern");
    Portrait p(0, 0, renderer, w.getPortraitTexture());
    Portrait pt(350, 0, renderer, nullptr);
    QuestBook qb(850, 200, renderer);
    qb.addQuest(new Quest("First quest", "First quest in the world", 100, window, renderer));
    v_elements[0].push_back(dynamic_cast<Element*>(&w));
    v_elements[1].push_back(dynamic_cast<Element*>(&b2));
    v_elements[1].push_back(dynamic_cast<Element*>(&b));
    v_elements[1].push_back(dynamic_cast<Element*>(&b3));
    v_elements[1].push_back(dynamic_cast<Element*>(&b4));
    v_elements[1].push_back(dynamic_cast<Element*>(&npc));

    //--- Personnage au milieu de l'écran ---//
    w.addXOffset(1920 / 2 - 125 - posx);
    w.addYOffset(1080 / 2 - 125 - posy);
    w.resetPos();

    sort(v_elements[1].begin(), v_elements[1].end(), compareZ);

    // Variables pour le calcul du delta time
    thread t_player(t_move_player, &w);
    thread t_spell;
    thread t_aa;
    thread t_camera(t_update_camera, &w);
    bool playerDrawn = false;
    SDL_RenderClear(renderer);
    SDL_SetWindowSize(window, 1920, 1080);
    SDL_SetWindowPosition(window, 0, 0);

    // Activer la capture de la souris dans la fenêtre
    SDL_SetWindowGrab(window, SDL_TRUE);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);    
    while (run) 
    {
        sort(v_elements[1].begin(), v_elements[1].end(), compareZ);
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
                    if (events.key.keysym.sym == SDLK_SPACE) { for (vector<Element*> v : v_elements) for (Element* e : v) e->resetPos(); cameraLock = true; w.updateHitbox(); }
                    if (events.key.keysym.sym == SDLK_y) { cameraLock = !cameraLock; for (vector<Element*> v : v_elements) for (Element* e : v) e->resetPos(); w.updateHitbox(); }
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
                    
                    //cout << mouseX << " : " << mouseY << endl;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    bool targetFound = false;
                    if (events.button.button == SDL_BUTTON_LEFT)
                        for(vector<Element*> v_e : v_elements)
                        for (Element* e : v_e) 
                            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->inClickBox(events.button.x, events.button.y))
                            {
                                pt.setTextEntity(dynamic_cast<Entity*>(e)->getPortraitTexture()); targetFound = true;  break;
                            }
                        if (!targetFound) pt.setTextEntity(nullptr);
                    break;
            }
        }

        SDL_RenderClear(renderer);
        
        for (int i = 0; i < v_elements[1].size(); i++)
        {
            if(!playerDrawn)
                if (!(v_elements[1])[i]->isInFront(w.getXHitbox(), w.getYHitbox()))
                    (v_elements[1])[i]->draw(renderer);
                else
                {
                    w.draw(renderer);
                    playerDrawn = true;
                    (v_elements[1])[i]->draw(renderer);
                }
            else
                (v_elements[1])[i]->draw(renderer);
        }
        if(!playerDrawn) w.draw(renderer);
        playerDrawn = false;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        draw_circle(renderer, w.getXHitbox() + w.getSpeed() * w.getXRate(), w.getYHitbox() + w.getSpeed() * w.getYRate(), 10);

        //--- DRAW UI ---//
        p.draw(renderer);
        pt.draw(renderer);
        if(qb.isToDraw()) qb.draw(renderer);

        //--------------//

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, npc.getPClickBox());
        SDL_RenderDrawRect(renderer, w.getPClickBox());
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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