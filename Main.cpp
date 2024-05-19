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
#include <mutex>

#include "Map.h";
#include "MapFragment.h"
#include "Warrior.h"
#include "NPC.h"
#include "Building.h"
#include "UI.h"
#include "Font.h"

using namespace std;

mutex mtx;

SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };


SDL_bool run = SDL_TRUE;
Uint32 flags;
float deltaTime = 0;

int width = 1920, height = 1080;

Warrior* c;
UI* ui;
Map m(10, 10);
MapFragment* mf = nullptr;
vector<vector<Element*>> v_elements = { {}, {} };

float posi_cameraSpeed = 5, nega_cameraSpeed = -posi_cameraSpeed;
bool cameraLock = false;
int mouseX = -1, mouseY = -1;

void t_move_player();
void t_run_spell(Spell* spell);
void t_run_aa(AutoAttack* spell);
void t_run_screenMsg();
void t_update_camera();
void draw_circle(SDL_Renderer* renderer, int center_x, int center_y, int radius);
bool compareZ(Element* e1, Element* e2);
void handleLeftClick(SDL_Event& events , thread& t_screenMsg);
void handleRightClick(SDL_Event& events, thread& t_screenMsg);
void resetAllElementsPos();
void t_run_handleKeyEvents();
const int FPS = 60;
const int FRAME_TIME = 1000 / FPS;
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

    //if (SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1") == SDL_FALSE) {
    //    std::cerr << "Impossible d'activer la VSync : " << SDL_GetError() << std::endl;
    //}

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
    
    mf = new MapFragment("grass", renderer);
    int posx = 850, posy = 600;
    c = new Warrior("Titus", posx, posy, uti::Category::PLAYER, renderer);
    c->setXYMap(posx, posy);
    NPC npc("DENT", 900, 300, uti::Category::NPC, "character/warrior", false, renderer);
    ui = new UI(window, renderer, c);

    Building b(0, 0, 1000, 672, renderer, "tavern/tavern");
    Building b2(250, 1500, 1000, 672, renderer, "tavern/tavern");
    Building b3(1000, 500, 1000, 672, renderer, "tavern/tavern");
    Building b4(1500, 800, 1000, 672, renderer, "tavern/tavern");
    
    //qb.addQuest(new Quest("First quest", "First quest in the world", 100, window, renderer));
    v_elements[0].push_back(dynamic_cast<Element*>(c));
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

    //--- Personnage au milieu de l'écran ---//
    c->addXOffset(width / 2 - 125 - posx);
    c->addYOffset(height / 2 - 125 - posy);
    c->resetPos();

    //--- Déplacement des fragments de map par rapport au personnage ---//
    //m.addOffset(width / 2 - 125 - posx, height / 2 - 125 - posy);
    //m.resetPos();
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
        {
            m.addFragment(i, j, "grass", renderer);
            m.setFragmentVisible(i, j, true);
            m.addOffset(i, j, width / 2 - 125 - posx + j * 1920, height / 2 - 125 - posy + i * 1080);
            m.resetPos();
        }

    //m.setUp(width / 2 - 125 - posx, height / 2 - 125 - posy);

    //m.showOffset();

    sort(v_elements[1].begin(), v_elements[1].end(), compareZ);

    // Variables pour le calcul du delta time
    thread t_player(t_move_player);
    thread t_spell;
    thread t_aa;
    thread t_screenMsg;
    thread t_camera(t_update_camera);
    
    SDL_RenderClear(renderer);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, 0, 0);

    // Activer la capture de la souris dans la fenêtre
    SDL_SetWindowGrab(window, SDL_TRUE);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    SDL_Event events;
    //thread t_handleKeyEvents(t_run_handleKeyEvents);
    Uint32 startTime;
    int frameTime;
    bool playerDrawn = false;
    //c->setMoving(true);
    //c->countDir += uti::Direction::LEFT;  c->left = true;  c->update();
    //cameraLock = true;
    while (run) 
    {
        startTime = SDL_GetTicks();
        //cout << c->getXMap() << " : " << c->getYMap() << endl;
        sort(v_elements[1].begin(), v_elements[1].end(), compareZ);
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
                case SDL_WINDOWEVENT: if (events.window.event == SDL_WINDOWEVENT_CLOSE) run = SDL_FALSE; break;
                case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué
                    //--- Déplacements ---//
                    if (events.key.keysym.sym == SDLK_z) { if (!c->up)    c->countDir += uti::Direction::UP;    c->up = true;    c->update(); }
                    if (events.key.keysym.sym == SDLK_d) { if (!c->right) c->countDir += uti::Direction::RIGHT; c->right = true; c->update(); }
                    if (events.key.keysym.sym == SDLK_s) { if (!c->down)  c->countDir += uti::Direction::DOWN;  c->down = true;  c->update(); }
                    if (events.key.keysym.sym == SDLK_q) { if (!c->left)  c->countDir += uti::Direction::LEFT;  c->left = true;  c->update(); }
                    //--- Spells ---//
                    if (events.key.keysym.sym == SDLK_a) { if (!c->isSpellActive()) { c->setCancelAA(true); if (!c->isSpellActive()) { if (t_spell.joinable()) t_spell.join(); t_spell = thread(t_run_spell, c->getSpell(1)); } } }
                    if (events.key.keysym.sym == SDLK_e) { if (!c->isSpellActive()) { if (!c->isAAActive()) { if (t_aa.joinable()) t_aa.join(); t_aa = thread(t_run_aa, dynamic_cast<AutoAttack*>(c->getSpell(0))); } } }

                //    //--- Caméra ---//
                    if (events.key.keysym.sym == SDLK_SPACE) { cameraLock = true;        resetAllElementsPos(); c->updateMovebox(); }
                    if (events.key.keysym.sym == SDLK_y) { cameraLock = !cameraLock; resetAllElementsPos(); c->updateMovebox(); }
                    break;
                case SDL_KEYUP: // Un événement de type touche relâchée est effectué                
                    if (events.key.keysym.sym == SDLK_ESCAPE) { run = SDL_FALSE; c->setAlive(false); c->setCancelAA(true); }
                    if (events.key.keysym.sym == SDLK_z) { if (c->up)    c->countDir -= uti::Direction::UP;     c->up = false;    c->update(); }
                    if (events.key.keysym.sym == SDLK_d) { if (c->right) c->countDir -= uti::Direction::RIGHT;  c->right = false; c->update(); }
                    if (events.key.keysym.sym == SDLK_q) { if (c->left)  c->countDir -= uti::Direction::LEFT;   c->left = false;  c->update(); }
                    if (events.key.keysym.sym == SDLK_s) { if (c->down)  c->countDir -= uti::Direction::DOWN;   c->down = false;  c->update(); }
                    if (events.key.keysym.sym == SDLK_SPACE) cameraLock = false;

                //    //--- UI SHORTCUTS ---//
                    if (events.key.keysym.sym == SDLK_l) ui->setQBVisible(!ui->isQBVisible());
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = events.motion.x;   mouseY = events.motion.y;
                    break;
            case SDL_MOUSEBUTTONDOWN:
                cout << mouseX << " : " << mouseY << endl;
                if (events.button.button == SDL_BUTTON_LEFT)  handleLeftClick(events, t_screenMsg);
                else if (events.button.button == SDL_BUTTON_RIGHT) handleRightClick(events, t_screenMsg);
                break;
            }
        }
      
        mtx.lock();
        //cout << "render lock" << endl;
        SDL_RenderClear(renderer);
        //mf->draw(renderer);
        m.draw(renderer);

        for (int i = 0; i < v_elements[1].size(); i++)
        {
            if (!playerDrawn)
                if (!(v_elements[1])[i]->isInFront(c->getXMovebox(), c->getYMovebox()))
                    (v_elements[1])[i]->draw(renderer);
                else
                {
                    c->draw(renderer);
                    playerDrawn = true;
                    (v_elements[1])[i]->draw(renderer);
                }
            else
                (v_elements[1])[i]->draw(renderer);
        }
        if (!playerDrawn)c->draw(renderer);
        playerDrawn = false;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        draw_circle(renderer, c->getXMovebox()/* + c->getSpeed() * c->getXRate()*/, c->getYMovebox()/* + c->getSpeed() * c->getYRate()*/, 10);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawRect(renderer, npc.getPClickBox());
        SDL_RenderDrawRect(renderer, c->getPClickBox());

        //--- DRAW UI ---//
        ui->draw(renderer);
        //--------------//
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - startTime;

        //// Limiter les FPS en attendant le temps restant
        //if (frameTime < FRAME_TIME) {
        //    SDL_Delay(FRAME_TIME - frameTime);
        //    //cout << FRAME_TIME - frameTime << endl;
        //}
        //cout << "render unlock" << endl;
        mtx.unlock();
        SDL_Delay(1);

        //cout << "MAJ" << endl;
        //cout << SDL_GetTicks() - startTime << endl;
    }

    if(t_player.joinable())  t_player.join();
    if(t_spell.joinable())   t_spell.join();
    if(t_aa.joinable())      t_aa.join();
    if (t_camera.joinable()) t_camera.join();
    if (t_screenMsg.joinable()) t_screenMsg.join();
    //if (t_handleKeyEvents.joinable()) t_handleKeyEvents.join();

    SDL_FreeSurface(loading_screen_img);
    SDL_DestroyTexture(loading_screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete mf;
    mf = nullptr;

    delete c;
    c = nullptr;

    return 0;
}

void t_run_handleKeyEvents()
{
    SDL_Event events;
    while (true)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            //case SDL_WINDOWEVENT: if (events.window.event == SDL_WINDOWEVENT_CLOSE) run = SDL_FALSE; break;
            //case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué
            //    //--- Déplacements ---//
            //    if (events.key.keysym.sym == SDLK_z) { if (!c->up)    c->countDir += uti::Direction::UP;    c->up = true;    c->update(); }
            //    if (events.key.keysym.sym == SDLK_d) { if (!c->right) c->countDir += uti::Direction::RIGHT; c->right = true; c->update(); }
            //    if (events.key.keysym.sym == SDLK_s) { if (!c->down)  c->countDir += uti::Direction::DOWN;  c->down = true;  c->update(); }
            //    if (events.key.keysym.sym == SDLK_q) { if (!c->left)  c->countDir += uti::Direction::LEFT;  c->left = true;  c->update(); }
            //    //--- Spells ---//
            //    //if (events.key.keysym.sym == SDLK_a) { if (!c->isSpellActive()) { c->setCancelAA(true); if (!c->isSpellActive()) { if (t_spell.joinable()) t_spell.join(); t_spell = thread(t_run_spell, c->getSpell(1)); } } }
            //    //if (events.key.keysym.sym == SDLK_e) { if (!c->isSpellActive()) { if (!c->isAAActive()) { if (t_aa.joinable()) t_aa.join(); t_aa = thread(t_run_aa, dynamic_cast<AutoAttack*>(c->getSpell(0))); } } }

            //    //--- Caméra ---//
            //    if (events.key.keysym.sym == SDLK_SPACE) { cameraLock = true;        resetAllElementsPos(); c->updateMovebox(); }
            //    if (events.key.keysym.sym == SDLK_y) { cameraLock = !cameraLock; resetAllElementsPos(); c->updateMovebox(); }
            //    break;
            //case SDL_KEYUP: // Un événement de type touche relâchée est effectué                
            //    if (events.key.keysym.sym == SDLK_ESCAPE) { run = SDL_FALSE; c->setAlive(false); c->setCancelAA(true); }
            //    if (events.key.keysym.sym == SDLK_z) { if (c->up)    c->countDir -= uti::Direction::UP;     c->up = false;    c->update(); }
            //    if (events.key.keysym.sym == SDLK_d) { if (c->right) c->countDir -= uti::Direction::RIGHT;  c->right = false; c->update(); }
            //    if (events.key.keysym.sym == SDLK_q) { if (c->left)  c->countDir -= uti::Direction::LEFT;   c->left = false;  c->update(); }
            //    if (events.key.keysym.sym == SDLK_s) { if (c->down)  c->countDir -= uti::Direction::DOWN;   c->down = false;  c->update(); }
            //    if (events.key.keysym.sym == SDLK_SPACE) cameraLock = false;

            //    //--- UI SHORTCUTS ---//
            //    if (events.key.keysym.sym == SDLK_l) ui->setQBVisible(!ui->isQBVisible());
            //    break;
            //case SDL_MOUSEMOTION:
            //    mouseX = events.motion.x;   mouseY = events.motion.y;
            //    break;
            case SDL_MOUSEBUTTONDOWN:
                //cout << mouseX << " : " << mouseY << endl;
                cout << "OKK" << endl;
                //if (events.button.button == SDL_BUTTON_LEFT)  handleLeftClick(events, t_screenMsg);
                //else if (events.button.button == SDL_BUTTON_RIGHT) handleRightClick(events, t_screenMsg);
                break;
            }
        }
        SDL_Delay(1);
    }
}

void t_move_player()
{
    Uint32 lastTime = SDL_GetTicks64();
    Uint32 currentTime;
    float deltaTime;
    
    while (c->isAlive())
    {
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        if (c->isMoving() && !c->isSpellActive()) { mtx.lock(); c->move(v_elements[1], m, cameraLock, deltaTime); mtx.unlock(); }
        SDL_Delay(1);
    }
}

void t_run_spell(Spell* spell)
{
    spell->run(v_elements[1], *c, m, cameraLock);
    if (c->getCountDir() == 0) c->setMoving(false);
}

void t_run_aa(AutoAttack* spell)
{
    while (!c->getCancelAA())
    {
        spell->run(v_elements[1], *c, m, cameraLock);
        if (c->getCountDir() == 0) c->setMoving(false);
        for (int p = 0; p < 1000; p++)
        {
            if (c->getCancelAA()) break;
            Sleep(1);
        }
    }
    c->setAAActive(false);
}

void t_run_screenMsg()
{
    for (int i = 0; i < 2000 && run; i++) { SDL_Delay(1); }
    ui->setScreenMsgVisible(false);
}

void t_update_camera()
{
    while (run)//trouver un moyen de synchro avec le move du joueur
    {
        flags = SDL_GetWindowFlags(window);
        if (flags & SDL_WINDOW_INPUT_FOCUS)
            if (!cameraLock)
            {
                if (mouseX == 0)
                {
                    mtx.lock();
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateXOffset(posi_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }

                    mf->updateXOffset(posi_cameraSpeed);
                    m.updateXOffset(posi_cameraSpeed);
                    mtx.unlock();
                }//on déplace la caméra dans un sens et on enregistre l'offset dans l'autre sens pour revenir au point de départ
                else if (mouseX >= 1919)
                {
                    mtx.lock();
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateXOffset(nega_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }

                    mf->updateXOffset(nega_cameraSpeed);    
                    m.updateXOffset(-posi_cameraSpeed);
                    mtx.unlock();
                }
                if (mouseY == 0)
                {
                    mtx.lock();
                    //--- Mise à jour des éléments ---//
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateYOffset(posi_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }

                    //--- Mise à jour de la position des fragments de map ---//
                    mf->updateYOffset(posi_cameraSpeed);
                    m.updateYOffset(posi_cameraSpeed);

                    mtx.unlock();
                }
                else if (mouseY >= 1079)
                {
                    mtx.lock();
                    //--- Mise à jour des éléments ---//
                    for (vector<Element*> v : v_elements)
                        for (Element* e : v)
                        {
                            e->updateYOffset(nega_cameraSpeed);
                            if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateMovebox(); dynamic_cast<Entity*>(e)->updateClickBox(); }
                        }
                    //--- Mise à jour de la position des fragments de map ---//
                    mf->updateYOffset(nega_cameraSpeed);
                    m.updateYOffset(-posi_cameraSpeed);
                    mtx.unlock();
                }
            }
        //else for(Element* e : *(v_elements)[1]) e->resetPos();//applique le offset aux elements du décors
        SDL_Delay(1);
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

void handleLeftClick(SDL_Event& events, thread& t_screenMsg)
{
    bool targetFound = false;
    for (vector<Element*> v_e : v_elements)
        for (Element* e : v_e)
            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->inClickBox(events.button.x, events.button.y))
            {
                ui->setTargetPortrait(dynamic_cast<Entity*>(e)->getPortraitTexture()); targetFound = true;
                break;
            }
    if (!targetFound) ui->setTargetPortrait(nullptr);
}

void handleRightClick(SDL_Event& events, thread& t_screenMsg)
{
    bool targetFound = false;
    for (vector<Element*> v_e : v_elements)
        for (Element* e : v_e)
            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->inClickBox(events.button.x, events.button.y))
            {
                ui->setTargetPortrait(dynamic_cast<Entity*>(e)->getPortraitTexture()); targetFound = true;
                if (!dynamic_cast<Entity*>(e)->isInClickRange(c->getXMovebox(), c->getYMovebox()))
                {

                    if (!ui->isScreenMsgVisible()) { if (t_screenMsg.joinable()) t_screenMsg.join(); ui->setScreenMsgVisible(true); t_screenMsg = thread(t_run_screenMsg); }
                    break;
                }

                if (dynamic_cast<NPC*>(e)) { ui->getQuestBook()->addQuest(dynamic_cast<NPC*>(e)->getPQuest()); dynamic_cast<NPC*>(e)->getPQuest()->setState(2); }
                break;
            }
    if (!targetFound) ui->setTargetPortrait(nullptr);
}

void resetAllElementsPos()
{
    for (vector<Element*> v_e : v_elements) for (Element* e : v_e) e->resetPos();
    m.resetPos();
    mf->resetPos();
}
