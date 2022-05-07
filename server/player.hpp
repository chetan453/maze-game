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

        void intToByte(int n, unsigned char* result) {

            result[0] = n & 0x000000ff;
            result[1] = (n & 0x0000ff00) >> 8;
            result[2] = (n & 0x00ff0000) >> 16;
            result[3] = (n & 0xff000000) >> 24; 
        }


        inline void move(SDL_Renderer* renderer,ENetPeer* peer1, const bool testf) noexcept {

            unsigned char tobesent[10];
            tobesent[9]='0';
            if (inputs->get_press(input_keys::f,type)){
            // if(power_up>0){
            //     Mix_PlayChannel(se_type::siren, sound_manager_->get_se(se_type::siren),0);
            //     tobesent[9]='1';
            //     power_mode = 400;
            //     power_up--;
            // }
            
            }
            tobesent[0]='P';
            intToByte(pos.x,&tobesent[1]);
            intToByte(pos.y,&tobesent[5]);
            if(testf){
                ENetPacket* packet = enet_packet_create (tobesent, 
                                                10 ,0);
                enet_peer_send (peer1, 0, packet);
            }

            // const Point dst_pos = {nxt_.x*block::size, nxt_.y*block::size};

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

        inline void set_pos(const Point &p){pos = p; /*curr_ = {p.x/block::size, p.y/block::size}; nxt_ = curr_;*/}

        Point get_pos() {return pos;}
        int get_type(){return type;}
};