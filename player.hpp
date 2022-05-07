#pragma once 

#include "global.hpp"
#include "image.hpp"
#include "input.hpp"
#include "maze.hpp"
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdlib.h>
#include<time.h>
#include<memory>
using namespace std;

namespace player_hostels{
    enum{
        kumaon,
        jwala,
        girnar,
        nilgiri,
        himadri,
        count
    };
}

namespace player_hostels_pos{
    Point Kumaon_pos = {256,160};
    Point Girnar_pos = {456,160};
    Point Jwala_pos = {128,176};
    Point Nilgiri_pos = {128,544};
    Point Himadri_pos= {1144,160};
}

namespace player_type{
    enum{
        p1,
        p2
    };
}

int* get2rand(){
    int curr1 = rand()%player_hostels::count;
    int curr2 = rand()%player_hostels::count;

    while(curr1==curr2){
        cout<<curr2<<" "<<curr1<<endl; 
        curr2 = rand()%player_hostels::count;
    }

    int randarr[2];
    randarr[player_type::p1] = curr1;
    randarr[player_type::p2] = curr2;

    return randarr;
}

class Player{
    Point pos;
    int curr;
    int type;

    unsigned int speed;

    SDL_Renderer* p_renderer;
    const Image* _image_;
    const Input* inputs;
    public:
        Player(SDL_Renderer* renderer,const Image* image, const Input* input,int p_type):
        p_renderer(renderer),
        _image_(image),
        inputs(input),
        type(p_type){
            srand(time(0));            
            // curr = get2rand()[p_type];
            curr = (rand()+type)%player_hostels::count;
            switch(curr){
                case player_hostels::kumaon:{
                    pos = player_hostels_pos::Kumaon_pos;
                    break;
                }
                case player_hostels::jwala:{
                    pos = player_hostels_pos::Jwala_pos;
                    break;
                }
                case player_hostels::girnar:{
                    pos = player_hostels_pos::Girnar_pos;
                    break;
                }
                case player_hostels::nilgiri:{
                    pos = player_hostels_pos::Nilgiri_pos;
                    break;
                }
                case player_hostels::himadri:{
                    pos = player_hostels_pos::Himadri_pos;
                    break;
                }
                default:break;
            }
            speed = 4;
        }

        void draw(SDL_Renderer* renderer){
            switch(type){
                case player_type::p1:{
                    SDL_Rect rect = {pos.x,pos.y,8,8};
                    SDL_SetRenderDrawColor(renderer,0x7F,0x3C,0x98,0xFF);
                    SDL_RenderFillRect(renderer,&rect);
                    break;
                }
                case player_type::p2:{
                    // cout<<"p2 "<<pos.x<<" "<<pos.y<<endl;
                    SDL_Rect rect = {pos.x,pos.y,8,8};
                    SDL_SetRenderDrawColor(renderer,0x27,0xAE,0x60,0xFF);
                    SDL_RenderFillRect(renderer,&rect);
                    break;
                }
                default:break;
            }
        }

        inline void move(SDL_Renderer* renderer) noexcept {
            if((inputs->get_press(input_keys::up,type) || inputs->get_press(input_keys::w,type)) && isPath(pos.x,pos.y-speed,speed)){
                pos.y-=speed;
            }else
            if((inputs->get_press(input_keys::down,type) || inputs->get_press(input_keys::s,type)) && isPath(pos.x,pos.y+speed,speed)){
                
                pos.y+=speed;
            } else 
            if((inputs->get_press(input_keys::left,type) || inputs->get_press(input_keys::a,type)) && isPath(pos.x-speed,pos.y,speed)){
                pos.x-=speed;
            }else
            if((inputs->get_press(input_keys::right,type) || inputs->get_press(input_keys::d,type)) && isPath(pos.x+speed,pos.y,speed)){
                pos.x+=speed;
            }
            
            draw(renderer);

        }

        Point get_pos() {return pos;}
        int get_type(){return type;}
};