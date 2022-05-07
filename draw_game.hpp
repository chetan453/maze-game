#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include "game.hpp"

void Game::draw_game(){
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
    p1->draw(renderer);
    p2->draw(renderer);

    p1->move(renderer);
    p2->move(renderer);

    Point p1_pos = {1185,5};
    Point p1_task_pos = {1186,17};
    Point p1_tast_completd_pos = {1186,32};

    SDL_Rect tmp = {1180,0,100,50};
    SDL_SetRenderDrawColor(renderer,0xFF,0xA5,0x00,0xFF);
    SDL_RenderDrawRect(renderer,&tmp);

    text("Player 1",font_size::x16,p1_pos,RGB{0xAB,0x23,0x00});
    text(t1->get_task_text(),font_size::x8,p1_task_pos,RGB{0x22,0x45,0xAB});
    text("Task completed:" + to_string(t1->completed_size()),font_size::x8,p1_tast_completd_pos,RGB{0x22,0x45,0xAB});
            
    
    Point p2_pos = {1185,55};
    Point p2_task_pos = {1186,72};
    Point p2_tast_completd_pos = {1186,85};

    tmp = {1180,50,100,100};
    SDL_SetRenderDrawColor(renderer,0xFF,0xA5,0x00,0xFF);
    SDL_RenderDrawRect(renderer,&tmp);

    text("Player 2",font_size::x16,p2_pos,RGB{0xAB,0x23,0x00});
    text(t2->get_task_text(),font_size::x8,p2_task_pos,RGB{0x22,0x45,0xAB});
    text("Task completed:" + to_string(t2->completed_size()),font_size::x8,p2_tast_completd_pos,RGB{0x22,0x45,0xAB}); 

    Point p1_dst = t1->get_dst();
    Point p2_dst = t2->get_dst();

    tmp = {p1_dst.x-4,p1_dst.y-4,8,8};
    SDL_RenderFillRect(renderer,&tmp);

    tmp = {p2_dst.x-4,p2_dst.y-4,8,8};
    SDL_RenderFillRect(renderer,&tmp);

}