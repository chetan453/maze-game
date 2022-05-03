#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
// #include <SDL2/SDL_ttf.h>

using namespace std;

const int SCREEN_WIDTH=720;
const int SCREEN_HEIGHT=640;

const int IMAGE_HEIGHT=243;
const int IMAGE_WIDTH=543;

const int RECT_SIZE=9;
const float ANIMATION_WIDTH=35;
const float ANIMATION_HEIGHT=59;
SDL_Window* gwindow;
SDL_Renderer* renderer;
SDL_Texture* texture;

bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        cout<<"cannot initialiaze sdl "<<SDL_GetError()<<endl;
        success = false;
    }else{
        gwindow = SDL_CreateWindow("Walking Man",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,0);
        if(gwindow == NULL){
            cout<<"cannot initialize window "<<SDL_GetError()<<endl;
            success = false;
        }else{
            renderer = SDL_CreateRenderer(gwindow,-1,SDL_RENDERER_PRESENTVSYNC);
            if(renderer == NULL){
                cout<<"renderer could not be created! "<<SDL_GetError()<<endl;
                success = false;
            }else{
                SDL_SetRenderDrawColor(renderer,12,12,12,0xFF);

                int imgFlags =  IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags)){
                    cout<<"could not load SDL_image "<<IMG_GetError()<<endl;
                    success = false;
                }
            }

        }
    }

    return success;
}

SDL_Texture* loadTexture(string str){
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(str.c_str());
    if(loadedSurface == NULL){
        cout<<"unable to load image "<<IMG_GetError()<<endl;
    }else{
        newTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
        if(newTexture == NULL){
            cout<<"unable to create texture from surface"<<SDL_GetError()<<endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadmedia(){
    bool success = true;
    texture = loadTexture("walking_man.png");
    if(texture == NULL){
        success = false;
    }
    return success;
}


void close(){
    SDL_DestroyTexture(texture);
    texture = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gwindow);
    renderer = NULL;
    gwindow = NULL;

    SDL_Quit();
    IMG_Quit();
}


int main(){
    if(init()){
        if(loadmedia()){
            bool quit = false;
            SDL_Event e;
            SDL_Rect tmp[RECT_SIZE];
            tmp[0].x=0;
            for(int i=0;i<RECT_SIZE;i++){
                // if(i>0)
                //     tmp[i].x=tmp[i-1].x+65;
                tmp[i].y=0;
                tmp[i].w=ANIMATION_WIDTH;
                tmp[i].h=ANIMATION_HEIGHT;
            }

            
            tmp[1].x=65;
            tmp[2].x=130;
            tmp[3].x=193;
            tmp[4].x=256;
            tmp[5].x=321;
            tmp[6].x=384;
            tmp[7].x=449;
            tmp[8].x=512;

            SDL_Rect dest={50,50,ANIMATION_WIDTH,ANIMATION_HEIGHT};
            int frames=0;
            while(!quit){
                while(SDL_PollEvent(&e)){
                    if(e.key.keysym.sym == SDLK_ESCAPE){
                        quit = true;
                    }
                }
                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer,texture,&tmp[frames/4],&dest);

                frames++;
                if(frames>35){
                    frames=0;
                }

                SDL_RenderPresent(renderer);
            }
        }
    }
    // cout<<ANIMATION_WIDTH<<" "<<ANIMATION_HEIGHT<<endl;
    close();
    return 0;
}