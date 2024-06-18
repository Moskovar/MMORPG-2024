#pragma once
#include "Connection.h"
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

#include "uti.h"
#include "Map.h";
#include "MapFragment.h"
#include "Warrior.h"
#include "NPC.h"
#include "Building.h"
#include "UI.h"
#include "Font.h"

#include <io.h>
#include <fcntl.h>

using namespace std;

Connection co;

mutex mtx;

SDL_Window* window{ nullptr };
SDL_Renderer* renderer{ nullptr };


SDL_bool run = SDL_TRUE;
Uint32 flags;
float deltaTime = 0;

int width = 1920, height = 1080, initialXOffset = 0, initialYOffset = 0;
Warrior* c = nullptr;
UI* ui = nullptr;
const int mfWidth = 4, mfHeight = 4;
Map m(mfWidth, mfHeight, renderer);
vector<vector<Element*>> v_elements = { {}, {} };//0 character  1 autres entities, 2????
vector<Element*> v_elements_solid, v_elements_depth;
map<int, Entity*> entities;

float cameraSpeed = 5;
bool cameraLock = false;
int mouseX = -1, mouseY = -1;

void t_move_player();
void t_move_players();
void t_run_screenMsg();
void t_update_camera();
void draw_circle(SDL_Renderer* renderer, int center_x, int center_y, int radius);
bool compareZ(Element* e1, Element* e2);
void handleLeftClick(SDL_Event& events , thread& t_screenMsg);
void handleRightClick(SDL_Event& events, thread& t_screenMsg);
void resetAllElementsPos();
void t_receive_data_udp();
void t_receive_data_TCP();
void applyInitialOffset(Entity* e);
void updateNetworkPlayer(Entity* e, const uti::NetworkEntity& ne);
void moveNetworkEntity(Entity* e);
void applyDamages(short dmg);

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

    uti::NetworkEntity ne         = { 0, 0, 0, 0, 0 };
    uti::NetworkEntitySpell nes   = {};
    uti::NetworkEntityFaction nef = {};
    uti::NetworkEntityTarget net  = {};
    co.recvTCP(ne, nes, nef, net, run);
    co.recvTCP(ne, nes, nef, net, run);
    if (ne.id == -1) { cout << "Le serveur est plein" << endl; exit(1); }//changer avec une fermeture prope du socket ??

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

    //cout << ne.id << " : " << ne.countDir << " : " << ne.x << " : " << ne.y << " : " << ne.timestamp << endl;

    float posx = ne.xMap / 100, posy = ne.yMap / 100;
    int rowMap = posy / 1080, colMap = posx / 1920;
    initialXOffset = width / 2 - 125 - ((int)posx % 1920);
    initialYOffset = height / 2 - 125 - ((int)posy % 1080);
    c = new Warrior("Titus", posx, posy, ne.id, 1, renderer);
    c->setHealth(ne.hp);
    c->setFaction(nef.faction);
    cout << "FACTION: " << c->getFaction() << endl;
    entities[ne.id] = c;
    c->setXYMap(posx, posy);

    ui = new UI(window, renderer, c);
    
    v_elements[0].push_back(dynamic_cast<Element*>(c));

    //--- Personnage au milieu de l'écran ---//
    c->addXOffset(initialXOffset);
    c->addYOffset(initialYOffset);
    c->resetPos();

    //--- Elements du vecteur ajustés au déplaçage du personnage au milieu de lécran---//
    for (Element* e : v_elements[1])
    {
        e->addXOffset(initialXOffset);
        e->addYOffset(initialYOffset);
        e->resetPos();
    }

    //--- Construction de la map ---//
    MapFragment* mf1 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(0, 0, mf1);                    {}
    MapFragment* mf2 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(0, 1, mf2);                    {}
    MapFragment* mf3 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(0, 2, mf3);                    {}
    MapFragment* mf4 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(0, 3, mf4);                    {}
                                                 {}
    MapFragment* mf5 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(1, 0, mf5);                    {}
    MapFragment* mf6 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(1, 3, mf6);

    MapFragment* mf7 = new MapFragment("grass", renderer, { new Building(-375, -500, 1920, 1080, renderer, "houses/house1")});
    m.addFragment(1, 1, mf7);
    MapFragment* mf8 = new MapFragment("grass", renderer,  {});
    m.addFragment(1, 2, mf8);
    MapFragment* mf9 = new MapFragment("grass", renderer,  {});
    m.addFragment(2, 1, mf9);
    MapFragment* mf10 = new MapFragment("grass", renderer, {});
    m.addFragment(2, 2, mf10);


    MapFragment* mf11 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(2, 0, mf11);
    MapFragment* mf12 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(2, 3, mf12);

    MapFragment* mf13 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(3, 0, mf13);
    MapFragment* mf14 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(3, 1, mf14);
    MapFragment* mf15 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(3, 2, mf15);
    MapFragment* mf16 = new MapFragment("enchantedforest", renderer, {});
    m.addFragment(3, 3, mf16);

    //--- Déplacement des fragments de map par rapport au personnage ---//
    for (int i = 0; i < mfWidth; i++)
        for (int j = 0; j < mfHeight; j++)
        {
            m.addOffset(i, j, initialXOffset + (j - colMap) * 1920, initialYOffset + (i - rowMap) * 1080);//on retire à i et j rowMap et colMap pour se placer au bon endroit
            m.resetPos();
        }

    v_elements_solid = m.getElements(true);
    v_elements_depth = m.getElements(false);
    v_elements_depth.push_back(c);

    sort(v_elements_depth.begin(), v_elements_depth.end(), compareZ);

    // Variables pour le calcul du delta time
    thread t_player(t_move_player);
    thread t_players(t_move_players);
    thread t_screenMsg;
    thread t_camera(t_update_camera);
    thread t_listen_tcp(t_receive_data_TCP);
    
    SDL_RenderClear(renderer);
    SDL_SetWindowSize(window, width, height);
    SDL_SetWindowPosition(window, 0, 0);

    //--- pour mettre à jour les boxes car problème sinon [à régler...] ---//
    mtx.lock();
    for (auto it = entities.begin(); it != entities.end(); ++it)
        it->second->updateBoxes();
    mtx.unlock();

    // Activer la capture de la souris dans la fenêtre
    SDL_SetWindowGrab(window, SDL_TRUE);
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    SDL_Event events;
    while (run)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
                case SDL_WINDOWEVENT: if (events.window.event == SDL_WINDOWEVENT_CLOSE) run = SDL_FALSE; break;
                case SDL_KEYDOWN: // Un événement de type touche enfoncée est effectué
                    //--- Déplacements ---//
                    if (events.key.keysym.sym == SDLK_z) { if (!c->up)    { c->countDir += uti::Direction::UP;    c->up    = true; c->update(); co.sendNETCP(c->getNE()); } }
                    if (events.key.keysym.sym == SDLK_d) { if (!c->right) { c->countDir += uti::Direction::RIGHT; c->right = true; c->update(); co.sendNETCP(c->getNE()); } }
                    if (events.key.keysym.sym == SDLK_s) { if (!c->down)  { c->countDir += uti::Direction::DOWN;  c->down  = true; c->update(); co.sendNETCP(c->getNE()); } }
                    if (events.key.keysym.sym == SDLK_q) { if (!c->left)  { c->countDir += uti::Direction::LEFT;  c->left  = true; c->update(); co.sendNETCP(c->getNE()); } }
                    //--- Spells ---//
                    if (events.key.keysym.sym == SDLK_a) 
                    { 
                        if ((!c->getSpellUsed() || (c->getSpellUsed() && c->getSpellUsed()->isCancelable())) && c->getSpell(uti::SpellID::WHIRLWIND)->isAvailable())//Si pas de spell actif ou un spell interruptible et le spell est dispo
                        { 
                            c->setAAActive(false); //créer fct useSpell
                            c->setSpell(uti::SpellID::WHIRLWIND);  
                            co.sendNESTCP(c->getNES(uti::SpellID::WHIRLWIND)); 
                            c->getSpell(uti::SpellID::WHIRLWIND)->start_cd(); 
                        } 
                    }
                    if (events.key.keysym.sym == SDLK_e) 
                    { 
                        if ((!c->getSpellUsed() || (c->getSpellUsed() && c->getSpellUsed()->isCancelable())) && c->getSpell(uti::SpellID::PUSH)->isAvailable() 
                           && c->getTarget() && c->getTarget()->getFaction() != c->getFaction() && c->getSpell(uti::SpellID::PUSH)->isInRange(c->getCenterBox(), c->getTarget()->getCenterBox()) && c->isInGoodDirection())//Si pas de spell actif ou un spell interruptible et le spell est dispo
                        {
                            c->setAAActive(false);
                            c->setSpell(uti::SpellID::PUSH);
                            co.sendNESTCP(c->getNES(uti::SpellID::PUSH));
                            c->getSpell(uti::SpellID::PUSH)->start_cd();
                        }
                    }
                    if (events.key.keysym.sym == SDLK_y) {  }

                    //--- Caméra ---//
                    if (events.key.keysym.sym == SDLK_SPACE) { cameraLock = true;        resetAllElementsPos(); c->updateMovebox(); }
                    if (events.key.keysym.sym == SDLK_y)     { cameraLock = !cameraLock; resetAllElementsPos(); c->updateMovebox(); }
                    if (events.key.keysym.sym == SDLK_r) applyDamages(5);
                    if (events.key.keysym.sym == SDLK_t) applyDamages(-5);
                    break;
                case SDL_KEYUP: // Un événement de type touche relâchée est effectué
                    //--- Quitter le jeu ---//
                    if (events.key.keysym.sym == SDLK_ESCAPE) { run = SDL_FALSE; c->setAlive(false); c->setCancelAA(true); }
                    //--- Déplacements ---//
                    if (events.key.keysym.sym == SDLK_z) { if (c->up)    c->countDir -= uti::Direction::UP;     c->up = false;    c->update(); co.sendNETCP(c->getNE()); }
                    if (events.key.keysym.sym == SDLK_d) { if (c->right) c->countDir -= uti::Direction::RIGHT;  c->right = false; c->update(); co.sendNETCP(c->getNE()); }
                    if (events.key.keysym.sym == SDLK_q) { if (c->left)  c->countDir -= uti::Direction::LEFT;   c->left = false;  c->update(); co.sendNETCP(c->getNE()); }
                    if (events.key.keysym.sym == SDLK_s) { if (c->down)  c->countDir -= uti::Direction::DOWN;   c->down = false;  c->update(); co.sendNETCP(c->getNE()); }
                    if (events.key.keysym.sym == SDLK_w) SDL_SetWindowPosition(window, -1920, 0);
                    if (events.key.keysym.sym == SDLK_x) SDL_SetWindowPosition(window, 1920, 0);
                    //--- Caméra ---//
                    if (events.key.keysym.sym == SDLK_SPACE) cameraLock = false;

                    //--- UI SHORTCUTS ---//
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
        SDL_RenderClear(renderer);
        m.draw(renderer);

        sort(v_elements_depth.begin(), v_elements_depth.end(), compareZ);

        //pas efficace avec tous les joueurs...
        for (Element* e : v_elements_depth)
        {
            if (dynamic_cast<Building*>(e))
            {
                dynamic_cast<Building*>(e)->getEntitiesInArea(entities);
                sort(dynamic_cast<Building*>(e)->entities.begin(), dynamic_cast<Building*>(e)->entities.end(), compareZ);
            }
        }

        for (Element* e : v_elements_depth)
        {
            if (dynamic_cast<Entity*>(e) && !e->isDrawn) { e->draw(renderer); e->isDrawn = true; }
            for (Element* e2 : v_elements_depth)
            {
                if (!dynamic_cast<Building*>(e2)) continue;
                if (!dynamic_cast<Building*>(e2)->isAllEntitiesDrawn()) break;
                if(!e2->isDrawn)
                {
                    e2->draw(renderer);
                    e2->isDrawn = true;
                }
            }
        }

        for (Element* e : v_elements_depth) e->isDrawn = false;

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        for(auto it = entities.begin(); it != entities.end(); ++it)
            if (it->second)
            {
                draw_circle(renderer, it->second->getXMovebox() + 5 * it->second->getSpeed() * it->second->getXRate() * it->second->getDeltaTime(), it->second->getYMovebox() + 5 * it->second->getSpeed() * it->second->getYRate() * it->second->getDeltaTime(), 10);
                draw_circle(renderer, it->second->getXCenterBox(), it->second->getYCenterBox(), 25);
            }
            draw_circle(renderer, c->getXMovebox() + 5 * c->getSpeed() * c->getXRate() * c->getDeltaTime(), c->getYMovebox() + 5 * c->getSpeed() * c->getYRate() * c->getDeltaTime(), 10);
            draw_circle(renderer, c->getXCenterBox(), c->getYCenterBox(), c->getCenterBoxRadius());

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawRect(renderer, npc.getPClickBox());
        SDL_RenderDrawRect(renderer, c->getPClickBox());

        //--- DRAW UI ---//
        ui->draw(renderer);
        //--------------//
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
        mtx.unlock();
        SDL_Delay(8);
    }

    if(t_player.joinable())     t_player.join();
    if(t_players.joinable())    t_players.join();
    if(t_camera.joinable())     t_camera.join();
    if(t_screenMsg.joinable())  t_screenMsg.join();
    if(t_listen_tcp.joinable()) t_listen_tcp.join();


    SDL_FreeSurface(loading_screen_img);
    SDL_DestroyTexture(loading_screen_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    if (c != nullptr)//déjà supprimé avec le map mais on sait jamais?
    {
        delete c;
        c = nullptr;
    }

    return 0;
}

void t_move_player()
{
    Uint32 lastTime = SDL_GetTicks64();
    Uint32 currentTime;
    float deltaTime;
    bool sendSpellData = false;
    
    vector<SpellEffect> spellEffects = {};
    vector<SpellEffect> spellJustHit = {};//les sorts qui viennent juste d'être effectués, reset à chaque itération

    chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();

    while (c->isAlive())
    {
        now = chrono::high_resolution_clock::now();
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        short eid = 0;
        //cout << v_elements_solid.size() << endl;
        mtx.lock();
        c->move(v_elements[1], v_elements_solid, m, cameraLock, deltaTime, sendSpellData, spellEffects, now);

        if (sendSpellData)       
        { 
            co.sendNETCP(c->getNE()); 
            co.sendNESTCP(c->getNES(0));                     
            sendSpellData = false;
        }

        for (int i = spellEffects.size() - 1; i >= 0; i--)//On check si un effet de sort a été ajouté
        {
            if (spellEffects[i].last_hit_time != now) break;//Si on tombe sur un effet de sort different de now, alors c'est qu'il vient pas d'être ajouté donc on sort, tous les suivants ne le seront pas

            co.sendNETCP(c->getNE()); 
            if(c->getSpellUsed()) co.sendNESETCP(c->getNESE(c->getSpellUsed()->getID(), spellEffects[spellEffects.size() - 1].entityID));
        }

        //on check si AA active, si CD OK et si distance OK + si good direction
        if (c->isAAActive() && c->getSpell(uti::SpellID::AA)->isAvailable() && c->getSpell(uti::SpellID::AA)->isInRange(c->getCenterBox(), c->getTarget()->getCenterBox()) && c->isInGoodDirection())
        {
            c->setSpell(uti::SpellID::AA);
            co.sendNESTCP(c->getNES(uti::SpellID::AA));
            c->getSpell(uti::SpellID::AA)->start_cd();
            co.sendNESETCP(c->getNESE(c->getSpellUsed()->getID(), c->getTarget()->getID()));
        }
            
		mtx.unlock();
        Sleep(1);
	}
    run = SDL_FALSE;
}

void t_move_players()
{
    Uint32 lastTime = SDL_GetTicks64();
    Uint32 currentTime;
    float deltaTime;
    short dir = 0, step = 0;
    while (run)
    {
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f; // Convert to seconds
        lastTime = currentTime;
        mtx.lock();
        for (Element* e : v_elements[1])
        {
            if (dynamic_cast<Entity*>(e))
            {
                Entity* e_cast = dynamic_cast<Entity*>(e);

                e_cast->update();
                e_cast->updateBoxes();
                if (e_cast->getXRate() == 0 && e_cast->getYRate() == 0 && !e_cast->getSpellUsed()) { e_cast->resetStep(); continue; }

                bool collision = false;

                float xChange = e_cast->getSpeed() * e_cast->getXRate() * deltaTime,
                      yChange = e_cast->getSpeed() * e_cast->getYRate() * deltaTime;

                if (e_cast->getSpellUsed())
                {
                    xChange *= e_cast->getSpellUsed()->getBoostSpeed();
                    yChange *= e_cast->getSpellUsed()->getBoostSpeed();
                }

                dir = e_cast->getDir(), step = e_cast->getFlatStep();

                for (Element* b : v_elements_solid) if (b->check_collisions(e_cast->getXMovebox() + 5 * xChange, e_cast->getYMovebox() + 5 * yChange)) { collision = true; break; }
                if (!collision)
                {
                    e_cast->setXYMap(e_cast->getXMap() + xChange, e_cast->getYMap() + yChange);
                    e_cast->setX(c->getX() - (c->getXMap() - e_cast->getXMap()));
                    e_cast->setY(c->getY() - (c->getYMap() - e_cast->getYMap()));
                    
                    //e_cast->updateBoxes();
                }

                //cout << e_cast->getAnimationID() << endl;

                if (!e_cast->getSpellUsed())
                {
                    if (dir == 0)
                        if (step < 9 * e_cast->getAnimationSpeed())  e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 1)
                        if (step < 11 * e_cast->getAnimationSpeed()) e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 2)
                        if (step < 8 * e_cast->getAnimationSpeed())  e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 3)
                        if (step < 11 * e_cast->getAnimationSpeed()) e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 0.5)
                        if (step < 11 * e_cast->getAnimationSpeed()) e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 1.5)
                        if (step < 11 * e_cast->getAnimationSpeed()) e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 2.5)
                        if (step < 11 * e_cast->getAnimationSpeed()) e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                    else if (dir == 3.5)
                        if (step < 11 * e_cast->getAnimationSpeed()) e_cast->increaseStep();
                        else                                         e_cast->resetStep();
                }
                else
                {
                    if (e_cast->getSpellUsed()) e_cast->getSpellUsed()->runOthers(*e_cast);
                }
            }
        }
        mtx.unlock();
        SDL_Delay(1);
    }
}

void t_receive_data_udp()
{
    uti::NetworkEntity ne;
    while (run)
    {
        co.recvNEUDP(ne);
        
        //c->setX((int)(ne.x / 100) % 1920);
        //c->setY((int)(ne.y / 100) % 1080);
    }
}

void t_receive_data_TCP()
{
    uti::NetworkEntity        ne  = { -1, 0, 0, 0, 0, 0 };
    uti::NetworkEntitySpell   nes = { -1, 0, 0 };
    uti::NetworkEntityFaction nef = { -1, 0, 0 };
    uti::NetworkEntityTarget  net = { -1, 0, 0 };

    while (run)
    {
        ne.header  = -1;
        nes.header = -1;
        nef.header = -1;
        net.header = -1;
        if (co.recvTCP(ne, nes, nef, net, run) && run);// cout << "TCP NE received: " << ne.id << " : " << (float)ne.xMap / 100 << " : " << (float)ne.yMap / 100 << " : " << ne.timestamp << endl;//pour le debug
        else { run = SDL_FALSE; break;  }
        
        //Si c'est une déconnexion
        if (ne.header != -1 && ne.timestamp == -1)
        {
            mtx.lock();
            int i_delete = -1, i_delete_depth = -1;

            for (int i = 0; i < v_elements[1].size(); i++)
                if (dynamic_cast<Entity*>(v_elements[1][i]))
                    if (dynamic_cast<Entity*>(v_elements[1][i])->getID() == ne.id)
                    {
                        i_delete = i;
                        break;
                    }

            for (int i = 0; i < v_elements_depth.size(); i++)
                if (dynamic_cast<Entity*>(v_elements_depth[i]))
                    if (dynamic_cast<Entity*>(v_elements_depth[i])->getID() == ne.id)
                    {
                        i_delete_depth = i;
                        break;
                    }

            if (entities[ne.id])
            {
                delete entities[ne.id];//on delete le pointeur
                entities[ne.id] = nullptr;
            }

            entities.erase(ne.id);//puis on le supprime de toutes les listes
            if(i_delete != -1) v_elements[1].erase(v_elements[1].begin() + i_delete);
            if(i_delete_depth != -1) v_elements_depth.erase(v_elements_depth.begin() + i_delete_depth);


            mtx.unlock();
            continue;
        }
        
        //si le personnage existe alors on le met à jour
        if (ne.header != -1 && entities[ne.id])
        {
            mtx.lock();
            updateNetworkPlayer(entities[ne.id], ne);
            mtx.unlock();
            continue;
        }
        
        //Si pas entré dans le if d'avant et qu'on entre ici, alors c'est la connexion d'un joueur
        if (ne.header != -1) //si c'est le personnage du joueur alors il est déjà mis à sa création au début
        { //connexion on l'ajoute aux listes
            mtx.lock();
            entities[ne.id] = new Warrior("Teeta", ne.xMap / 100, ne.yMap / 100, ne.id, 1, renderer);
            updateNetworkPlayer(entities[ne.id], ne);
            v_elements[1].push_back(entities[ne.id]);
            v_elements_depth.push_back(entities[ne.id]);
            mtx.unlock();
            continue;
        }

        //Si on reçoit l'utilisation d'un sort
        if (nes.header != -1 && entities[nes.id])
        {
            mtx.lock();
            if(entities[nes.id]) entities[nes.id]->setSpell(nes.spellID);
            cout << entities[nes.id]->getSpellUsed() << endl;
            mtx.unlock();
            continue;
        }
        
        //Si on reçoit le changement de target d'un joueur
        if (net.header != -1 && entities[net.id])
        {
            entities[net.id]->setTarget(entities[net.targetID]);
        }

        //Si on reçoit la mise à jour de faction d'un joueur
        if (nef.header != -1 && entities[nef.id])
        {
            mtx.lock();
            entities[nef.id]->setFaction(nef.faction);
            entities[nef.id]->setHealthImg(c->getFaction(), renderer);
            mtx.unlock();
            continue;
        }
    }
}

void applyInitialOffset(Entity* e)//useless af ?? use updateNetworkPlayer instead
{
    e->addXOffset(initialXOffset);
    e->addYOffset(initialYOffset);
    e->resetPos();
}

void updateNetworkPlayer(Entity* e, const uti::NetworkEntity& ne)
{
    e->setXYMap((float)ne.xMap / 100, (float)ne.yMap / 100);
    e->setHealth(ne.hp);
    e->setX(c->getX() - (c->getXMap() - e->getXMap()));
    e->setY(c->getY() - (c->getYMap() - e->getYMap()));
    e->countDir = ne.countDir;
    //if (e->getCountDir() != 0) e->setMoving(true);
    //else                       e->setMoving(false);
    //e->setSpell(ne.spell);
    e->updateBoxes();
    e->update();
}

void moveNetworkEntity(Entity* e)
{

}

void applyDamages(short dmg)
{
    c->takeDamages(dmg);
    co.sendNETCP(c->getNE());
}

void t_run_screenMsg()
{
    for (int i = 0; i < 2000 && run; i++) { SDL_Delay(1); }
    ui->setScreenMsgVisible(false);
}

void t_update_camera()
{
    int xMax = 1920 - 125, xMin = 0 - 125, yMax = 1080 - 125, yMin = 0 - 125;
    while (run)//trouver un moyen de synchro avec le move du joueur
    {
        flags = SDL_GetWindowFlags(window);
        if (flags & SDL_WINDOW_INPUT_FOCUS)
            //cout << c->getX() << endl;
        if (!cameraLock)
        {
            mtx.lock();
            if (mouseX == 0 && c->getX() + cameraSpeed < xMax)
            {
                for (vector<Element*> v : v_elements)
                    for (Element* e : v)
                    {
                        e->updateXOffset(cameraSpeed);
                        if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateBoxes(); }//cast en entity car ce sont des méthodes de Entity qui sont virtuelles pures pour Element 
                    }
                m.updateXOffset(cameraSpeed);
            }//on déplace la caméra dans un sens et on enregistre l'offset dans l'autre sens pour revenir au point de départ
            else if (mouseX >= 1919 && c->getX() - cameraSpeed > xMin)
            {
                for (vector<Element*> v : v_elements)
                    for (Element* e : v)
                    {
                        e->updateXOffset(-cameraSpeed);
                        if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateBoxes(); }//cast en entity car ce sont des méthodes de Entity qui sont virtuelles pures pour Element
                    }
                m.updateXOffset(-cameraSpeed);
            }
            if (mouseY == 0 && c->getY() + cameraSpeed < yMax)
            {
                //--- Mise à jour des éléments ---//
                for (vector<Element*> v : v_elements)
                    for (Element* e : v)
                    {
                        e->updateYOffset(cameraSpeed);
                        if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateBoxes(); }//cast en entity car ce sont des méthodes de Entity qui sont virtuelles pures pour Element
                    }

                //--- Mise à jour de la position des fragments de map ---//
                m.updateYOffset(cameraSpeed);
            }
            else if (mouseY >= 1079 && c->getY() - cameraSpeed > yMin)
            {
                //--- Mise à jour des éléments ---//
                for (vector<Element*> v : v_elements)
                    for (Element* e : v)
                    {
                        e->updateYOffset(-cameraSpeed);
                        if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateBoxes(); }//cast en entity car ce sont des méthodes de Entity qui sont virtuelles pures pour Element
                    }
                //--- Mise à jour de la position des fragments de map ---//
                m.updateYOffset(-cameraSpeed);
            }
            mtx.unlock();
        }

        if (c->getX() > xMax && c->getDir() == 1 || c->getX() < xMin && c->getDir() == 3)
        {
            for (vector<Element*> v : v_elements)
                for (Element* e : v)
                {
                    e->updateXOffset(-c->getXChange());
                    if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateBoxes(); }
                }
            m.updateXOffset(-c->getXChange());
            //cout << -c->getXChange() << endl;
        }
        if (c->getY() > yMax && c->getDir() == 2 || c->getY() < yMin && c->getDir() == 0)
        {
            //--- Mise à jour des éléments ---//
            for (vector<Element*> v : v_elements)
                for (Element* e : v)
                {
                    e->updateYOffset(-c->getYChange());
                    if (dynamic_cast<Entity*>(e)) { dynamic_cast<Entity*>(e)->updateBoxes(); }
                }
            //--- Mise à jour de la position des fragments de map ---//
            m.updateYOffset(-c->getYChange());
        }
        //else c->decreaseOffset();
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
    c->setAAActive(false);
    c->resetTarget();
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
        {
            if (dynamic_cast<Entity*>(e) && dynamic_cast<Entity*>(e)->inClickBox(events.button.x, events.button.y))
            {
                ui->setTargetPortrait(dynamic_cast<Entity*>(e)->getPortraitTexture());
                targetFound = true;
                c->setTarget(dynamic_cast<Entity*>(e));
                co.sendNETTCP({ uti::Header::NET, c->getID(), c->getTarget()->getID() });
                if(dynamic_cast<Entity*>(e) != c && dynamic_cast<Entity*>(e)->getFaction() != c->getFaction() && !c->getSpellUsed())
                    c->setAAActive(true);
                else
                {
                    c->setAAActive(false);
                    c->resetTarget();
                }
                if (!dynamic_cast<Entity*>(e)->isInClickRange(c->getXMovebox(), c->getYMovebox()))
                {
                    if (!ui->isScreenMsgVisible())
                    {
                        if (t_screenMsg.joinable()) t_screenMsg.join();
                        ui->setScreenMsgVisible(true);
                        t_screenMsg = thread(t_run_screenMsg);
                    }
                    break;
                }

                if (dynamic_cast<NPC*>(e)) { ui->getQuestBook()->addQuest(dynamic_cast<NPC*>(e)->getPQuest()); dynamic_cast<NPC*>(e)->getPQuest()->setState(2); }
                break;
            }
        }
    if (!targetFound)
    {
        ui->setTargetPortrait(nullptr);
        c->setAAActive(false);
        c->resetTarget();
    }
}

void resetAllElementsPos()
{
    for (vector<Element*> v_e : v_elements) 
        for (Element* e : v_e)
        {
            e->resetPos();
            //if (dynamic_cast<Entity*>(e)) dynamic_cast<Entity*>(e)->updateBoxes();
        }
  m.resetPos();
}
