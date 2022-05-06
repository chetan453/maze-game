#include "game.hpp"
#include "global.hpp"
#include "maze.hpp"
#include "image.hpp"
#include "font.hpp"
#include "input.hpp"
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<vector>

using namespace std;

bool Game::init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        cout<<"cannot initialiaze sdl "<<SDL_GetError()<<endl;
        success = false;
    }else{
        gWindow = SDL_CreateWindow("IIT Delhi World",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screen::width,screen::height,SDL_WINDOW_SHOWN);
        if(gWindow == NULL){
            cout<<"cannot initialize window "<<SDL_GetError()<<endl;
            success = false;
        }else{
            renderer = SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_PRESENTVSYNC);
            if(renderer == NULL){
                cout<<"renderer could not be created! "<<SDL_GetError()<<endl;
                success = false;
            }else{
                SDL_SetRenderDrawColor(renderer,12,12,12,0xFF);
                SDL_RenderClear(renderer);
                int imgFlags =  IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags)){
                    cout<<"could not load SDL_image "<<IMG_GetError()<<endl;
                    success = false;
                }

                if(TTF_Init() == -1){
                    cout<<"could not initialize TTF "<<TTF_GetError()<<endl;
                    success = false;
                }
            }

        }
    }
    return success;
}

void Game::text(string str,int size,Point &p,RGB color){
    SDL_Color col_ = {color.r,color.g,color.b,0xFF};
    SDL_Surface* font_surface = TTF_RenderUTF8_Blended(fonts.get(size),str.c_str(),col_);
    SDL_Texture* font_texture = SDL_CreateTextureFromSurface(renderer,font_surface);
    SDL_Rect src = {0,0,font_surface->w,font_surface->h};
    SDL_Rect dest;
    dest.x = p.x;
    dest.y = p.y;
    SDL_QueryTexture(font_texture, nullptr, nullptr, &dest.w, &dest.h);
    SDL_RenderCopy(renderer, font_texture, &src, &dest);
    SDL_FreeSurface(font_surface);
    SDL_DestroyTexture(font_texture);
}


void Game::game_title(){
    SDL_SetRenderDrawColor(renderer,0xB2,0xBA,0xBB,0xFF);
    SDL_RenderClear(renderer);

    Point title_pos = {100,100};
    Point start_pos = {300,275};
    Point  ins_pos = {300,375};

    switch(game_mode){
        case 0:{
            string title_text = "IIT Delhi World";

            text(title_text,font_size::x96,title_pos,RGB{0x11,0x7A,0x65});
            
            switch(selected){
                case 0:{
                    SDL_SetRenderDrawColor(renderer,0xE8,0xF8,0xF5,0x00);
                    SDL_Rect tmp = {290,280,430,80};
                    SDL_RenderFillRect(renderer,&tmp);
                    text("START",font_size::x48,start_pos,RGB{0x22,0x99,0x54});
                    text("INSTRUCTIONS",font_size::x48,ins_pos,RGB{0x22,0x99,0x54});
                    if(inputs->get_press(input_keys::enter,0) || inputs->get_press(input_keys::enter,1)){
                        game_mode++;
                    }
                    break;
                }
                case 1:{
                    SDL_SetRenderDrawColor(renderer,0xE8,0xF8,0xF5,0x00);
                    SDL_Rect tmp = {290,380,430,80};
                    SDL_RenderFillRect(renderer,&tmp);
                    text("START",font_size::x48,start_pos,RGB{0x22,0x99,0x54});
                    text("INSTRUCTIONS",font_size::x48,ins_pos,RGB{0x22,0x99,0x54});
                    if(inputs->get_press(input_keys::enter,0) || inputs->get_press(input_keys::enter,1)){
                        game_mode=3;
                    }
                    break;
                }
                default: break;
            }
       

            if(inputs->get_press(input_keys::down,0 ) || inputs->get_press(input_keys::down,1) ){
                selected = 1;
            } else if(inputs->get_press(input_keys::up,0 ) || inputs->get_press(input_keys::up,1) ){
                selected = 0;
            } 
            break;
        }

        case 1:{
            //to start the game
            _state = game_state::start;
            break;
        }

        case 3:{
            //to get instructions
            break;
        }
        default: break;

    }
}

void Game::game_start(){
    SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0xFF);
    SDL_RenderClear(renderer);
    vector<SDL_Rect> pathVector,miscVector,groundVector;
    SDL_Rect pathRect = {0,0,8,8};
    SDL_Rect miscRect={0,0,8,8};
    SDL_Rect groundRect={0,0,8,8};
    for(int i=0;i<ROW;i++){
        for(int j=0;j<COLUMN;j++){
            if(maze[i][j] == path){
                pathRect.x=j*8;
                pathRect.y=i*8;                    
                pathVector.push_back(pathRect);
            }
            if(maze[i][j] == miscellaneous){
                    miscRect.x=j*8;
                    miscRect.y=i*8;
                    miscVector.push_back(miscRect);
                }
                if(maze[i][j] == ground){
                    groundRect.x=j*8;
                    groundRect.y=i*8;
                    groundVector.push_back(groundRect);
                }
        }
    }

    for(int i=0;i<pathVector.size();i++){
        SDL_SetRenderDrawColor(renderer,0xC6,0xC4,0x11,SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer,&pathVector[i]);
    }

    for(int i=0;i<miscVector.size();i++){
        SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderFillRect(renderer,&miscVector[i]);
    }

    for(int i=0;i<groundVector.size();i++){
        SDL_SetRenderDrawColor(renderer,0x0A,0x4B,0x0A,0xFF);
        SDL_RenderFillRect(renderer,&groundVector[i]);
    }



}

void Game::game_play(){
    cout<<"game play"<<endl;
}

void Game::game_pause(){
    cout<<"game pause"<<endl;
}

void Game::game_over(){
    cout<<"game over"<<endl;
}