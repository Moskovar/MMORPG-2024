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

#include "MapFragment.h"
#include "Warrior.h"
#include "NPC.h"
#include "Building.h"
#include "UI.h"
#include "Font.h"

using namespace std;

SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };

SDL_bool run = SDL_TRUE;
Uint32 flags;
float deltaTime = 0;

int width = 1920, height = 1080;

vector<MapFragment*> v_mapFragments;
vector<vector<Element*>> v_elements = { {}, {} };

float posi_cameraSpeed = 5, nega_cameraSpeed = -posi_cameraSpeed;
bool cameraLock = false;
int mouseX = -1, mouseY = -1;

void t_move_player(Entity* e);
void t_run_spell(Spell* spell, Entity* e);
void t_run_aa(AutoAttack* spell, Entity* e);
void t_run_screenMsg(UI* ui, SDL_Renderer* renderer);
void t_update_camera(Entity* e);
void draw_circle(SDL_Renderer* renderer, int center_x, int center_y, int radius);
bool compareZ(Element* e1, Element* e2);
void handleLeftClick(SDL_Event& events, UI& ui, Entity& entity, thread& t_screenMsg);
void handleRightClick(SDL_Event& events, UI& ui, Entity& entity, thread& t_screenMsg);

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
    
    MapFragment mf("grass", renderer);
    v_mapFragments.push_back(&mf);
    int posx = 850, posy = 600;
    Warrior w("Titus", posx, posy, uti::Category::PLAYER, renderer);
    w.setXYMap(posx, posy);
    NPC npc("DENT", 900, 300, uti::Category::NPC, "character/warrior", false, renderer);
    UI ui(window, renderer, &w);

    Building b(0, 0, 1000, 672, renderer, "tavern/tavern");
    Building b2(250, 1500, 1000, 672, renderer, "tavern/tavern");
    Building b3(1000, 500, 1000, 672, renderer, "tavern/tavern");
    Building b4(1500, 800, 1000, 672, renderer, "tavern/tavern");
    
    //qb.addQuest(new Quest("First quest", "First quest in the world", 100, window, renderer));
    v_elements[0].push_back(dynamic_cast<Element*>(&w));
    v_elements[1].push_back(dynamic_cast<Element*>(&b2));
    v_elements[1].push_back(dynamic_cast<Element*>(&b));
    v_elements[1].push_back(dynamic_cast<Element*>(&b3));
    v_elements[1].push_back(dynamic_cast<Element*>(&b4));
    v_elements[1].push_back(dynamic_cast<Element*>(&npc));

    for (Element* e : v_elements[1])
    {
        e->addXOffset(width / 2 - posx);
        e->addYOffset(height / 2 - posy);
        e->resetPos();
    }

    //--- Personnage au milieu de l'�cran ---//
    w.addXOffset(width / 2 - 125 - posx);
    w.addYOffset(height / 2 - 125 - posy);
    w.resetPos();

    sort(v_elements[1].begin(), v_elements[1].end(), compareZ);

    // Variables pour le calcul du delta time
    thread t_player(t_move_player, &w);
    thread t_spell;
    thread t_aa;
    thread t_screenMsg;
    
    thread t_camera(t_update_camera, &w);
    bool playerDrawn = false;
    SDL_RenderClear(renderer);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, 0, 0);

    // Activer la capture de la souris dans la fen�tre
    SDL_SetWindowGrab(window, SDL_TRUE);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    SDL_Event events;
    while (run) 
    {
        //cout << w.getXMap() << " : " << w.getYMap() << endl;
        sort(v_elements[1].begin(), v_elements[1].end(), compareZ);
        while (SDL_PollEvent(&events)) 
        {
            switch (events.type)
            {
                case SDL_WINDOWEVENT: if (events.window.event == SDL_WINDOWEVENT_CLOSE) run = SDL_FALSE; break;
                case SDL_KEYDOWN: // Un �v�nement de type touche enfonc�e est effectu�
                    //--- D�placements ---//
                    if (events.key.keysym.sym == SDLK_z)     { if (!w.up)    w.countDir += uti::Direction::UP;    w.up = true;    w.update(); }
                    if (events.key.keysym.sym == SDLK_d)     { if (!w.right) w.countDir += uti::Direction::RIGHT; w.right = true; w.update(); }
                    if (events.key.keysym.sym == SDLK_s)     { if (!w.down)  w.countDir += uti::Direction::DOWN;  w.down = true;  w.update(); }
                    if (events.key.keysym.sym == SDLK_q)     { if (!w.left)  w.countDir += uti::Direction::LEFT;  w.left = true;  w.update(); }
                    //--- Spells ---//
                    if (events.key.keysym.sym == SDLK_a)     { if (!w.isSpellActive()) { w.setCancelAA(true); if (!w.isSpellActive()) { if (t_spell.joinable()) t_spell.join(); t_spell = thread(t_run_spell, w.getSpell(1), &w); } } }
                    if (events.key.keysym.sym == SDLK_e)     { if (!w.isSpellActive()) { if (!w.isAAActive()) { if (t_aa.joinable()) t_aa.join(); t_aa = thread(t_run_aa, dynamic_cast<AutoAttack*>(w.getSpell(0)), &w); } } }
                    
                    //--- Cam�ra ---//
                    if (events.key.keysym.sym == SDLK_SPACE) { for (vector<Element*> v : v_elements) for (Element* e : v) e->resetPos(); cameraLock = true; w.updateMovebox(); }
                    if (events.key.keysym.sym == SDLK_y)     { cameraLock = !cameraLock; for (vector<Element*> v : v_elements) for (Element* e : v) e->resetPos(); w.updateMovebox(); }
                    break;
                case SDL_KEYUP: // Un �v�nement de type touche rel�ch�e est effectu�                
                    if (events.key.keysym.sym == SDLK_ESCAPE) { run = SDL_FALSE; w.setAlive(false); w.setCancelAA(true); }
                    if (events.key.keysym.sym == SDLK_z)      { if (w.up)    w.countDir -= uti::Direction::UP;     w.up = false;    w.update(); }
                    if (events.key.keysym.sym == SDLK_d)      { if (w.right) w.countDir -= uti::Direction::RIGHT;  w.right = false; w.update(); }
                    if (events.key.keysym.sym == SDLK_q)      { if (w.left)  w.countDir -= uti::Direction::LEFT;   w.left = false;  w.update(); }
                    if (events.key.keysym.sym == SDLK_s)      { if (w.down)  w.countDir -= uti::Direction::DOWN;   w.down = false;  w.update(); }
                    if (events.key.keysym.sym == SDLK_SPACE) cameraLock = false;

                    //--- UI SHORTCUTS ---//
                    if (events.key.keysym.sym == SDLK_l) ui.setQBVisible(!ui.isQBVisible());
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = events.motion.x;   mouseY = events.motion.y;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    cout << mouseX << " : " << mouseY << endl;
                    if      (events.button.button == SDL_BUTTON_LEFT)  handleLeftClick (events, ui, w, t_screenMsg);
                    else if (events.button.button == SDL_BUTTON_RIGHT) handleRightClick(events, ui, w, t_screenMsg);
                    break;
            }
        }

        SDL_RenderClear(renderer);

        for (MapFragment* mf : v_mapFragments) mf->draw(renderer);
        
        for (int i = 0; i < v_elements[1].size(); i++)
        {
            if(!playerDrawn)
                if (!(v_elements[1])[i]->isInFront(w.getXMovebox(), w.getYMovebox()))
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

        draw_circle(renderer, w.getXMovebox() + w.getSpeed() * w.getXRate(), w.getYMovebox() + w.getSpeed() * w.getYRate(), 10);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, npc.getPClickBox());
        SDL_RenderDrawRect(renderer, w.getPClickBox());

        //--- DRAW UI ---//
        ui.draw(renderer);
        //--------------//
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
        Sleep(1);
    }

    if(t_player.joinable())  t_player.join();
    if(t_spell.joinable())   t_spell.join();
    if(t_aa.joinable())      t_aa.join();
    if (t_camera.joinable()) t_camera.join();
    if (t_screenMsg.joinable()) t_screenMsg.join();

    SDL_FreeSurface(loading_screen_img);
    SDL_DestroyTexture(loading_screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

void t_move_player(Entity* e)
{
    while (e->isAlive())
    {
        if (e->isMoving() && !e->isSpellActive()) e->move(v_elements[1], v_mapFragments,cameraLock);
        Sleep(1);
    }
}

void t_run_spell(Spell* spell, Entity* e)
{
    spell->run(v_elements[1], *e, v_mapFragments, cameraLock);
    if (e->getCountDir() == 0) e->setMoving(false);
}

void t_run_aa(AutoAttack* spell, Entity* e)
{
    while (!e->getCancelAA())
    {
        spell->run(v_elements[1], *e, v_mapFragments, cameraLock);
        if (e->getCountDir() == 0) e->setMoving(false);
        for (int p = 0; p < 1000; p++)
        {
            if (e->getCancelAA()) break;
            Sleep(1);
        }
    }
    e->setAAActive(false);
}

void t_run_screenMsg(UI* ui, SDL_Renderer* renderer)
{
    for (int i = 0; i < 2000 && run; i++) { Sleep(1); }
    ui->setScreenMsgVisible(false);
}

void t_update_camera(Entity* e)
{
    while (run)//trouver un moyen de synchro avec le move du joueur
    {
        flags = SDL_GetWindowFlags(window);
        if (flags & SDL_WINDOW_INPUT_FOCUS)
            if (!cameraLock)
            {
                if (mouseX == 0)
                {
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateXOffset(posi_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }

                    for(MapFragment* mf : v_mapFragments) mf->updateXOffset(posi_cameraSpeed);                            
                }//on d�place la cam�ra dans un sens et on enregistre l'offset dans l'autre sens pour revenir au point de d�part
                else if (mouseX >= 1919)
                {
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateXOffset(nega_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }

                    for (MapFragment* mf : v_mapFragments) mf->updateXOffset(nega_cameraSpeed);                            
                }
                if (mouseY == 0)
                {
                    //--- Mise � jour des �l�ments ---//
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateYOffset(posi_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }

                    //--- Mise � jour de la position des fragments de map ---//
                    for (MapFragment* mf : v_mapFragments) mf->updateYOffset(posi_cameraSpeed);

                            
                }
                else if (mouseY >= 1079)
                {
                    //--- Mise � jour des �l�ments ---//
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateYOffset(nega_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }
                    //--- Mise � jour de la position des fragments de map ---//
                    for (MapFragment* mf : v_mapFragments) mf->updateYOffset(nega_cameraSpeed);
                }
            }
        //else for(Element* e : *(v_elements)[1]) e->resetPos();//applique le offset aux elements du d�cors
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

void handleLeftClick(SDL_Event& events, UI& ui, Entity& entity, thread& t_screenMsg)
{
    bool targetFound = false;
    for (vector<Element*> v_e : v_elements)
        for (Element* e : v_e)
            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->inClickBox(events.button.x, events.button.y))
            {
                ui.setTargetPortrait(dynamic_cast<Entity*>(e)->getPortraitTexture()); targetFound = true;
                break;
            }
    if (!targetFound) ui.setTargetPortrait(nullptr);
}

void handleRightClick(SDL_Event& events, UI& ui, Entity& entity, thread& t_screenMsg)
{
    bool targetFound = false;
    for (vector<Element*> v_e : v_elements)
        for (Element* e : v_e)
            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->inClickBox(events.button.x, events.button.y))
            {
                ui.setTargetPortrait(dynamic_cast<Entity*>(e)->getPortraitTexture()); targetFound = true;
                if (!dynamic_cast<Entity*>(e)->isInClickRange(entity.getXMovebox(), entity.getYMovebox()))
                {

                    if (!ui.isScreenMsgVisible()) { if (t_screenMsg.joinable()) t_screenMsg.join(); ui.setScreenMsgVisible(true); t_screenMsg = thread(t_run_screenMsg, &ui, renderer); }
                    break;
                }

                if (dynamic_cast<NPC*>(e)) { ui.getQuestBook()->addQuest(dynamic_cast<NPC*>(e)->getPQuest()); dynamic_cast<NPC*>(e)->getPQuest()->setState(2); }
                break;
            }
    if (!targetFound) ui.setTargetPortrait(nullptr);
}
