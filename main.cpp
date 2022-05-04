#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<vector>
#include "maze.h"

using namespace std;

struct Rect{
    int x,y,w,h;
};

const int SCREEN_WIDTH=1280;
const int SCREEN_HEIGHT=720;

SDL_Window* gWindow = NULL;

SDL_Renderer* renderer = NULL;

SDL_Texture* texture = NULL;

bool init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        cout<<"cannot initialiaze sdl "<<SDL_GetError()<<endl;
        success = false;
    }else{
        gWindow = SDL_CreateWindow("IIT Delhi World",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_FULLSCREEN_DESKTOP);
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

SDL_Texture* LoadTexture(string str){
    SDL_Texture* newTexture;
    SDL_Surface* loadsurface = IMG_Load(str.c_str());
    if(loadsurface == NULL){
        cout<<"cannot load image "<<str<<" "<<IMG_GetError()<<endl;
    }else{
        newTexture = SDL_CreateTextureFromSurface(renderer,loadsurface);
        if(newTexture == NULL){
            cout<<"texture cannot be created "<<SDL_GetError()<<endl;
        }
    }
    return newTexture;
}

void render(string str,SDL_Rect rect){
    SDL_Texture* tmpTexture = LoadTexture(str);
    if(tmpTexture != NULL){
        SDL_RenderCopy(renderer,tmpTexture,NULL,&rect);
    }
}


void close(){
    SDL_DestroyTexture(texture);
    texture = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    renderer = NULL;
    gWindow = NULL;

    SDL_Quit();
    IMG_Quit();
}

int main(){
    vector<SDL_Rect> pathVector,miscVector,groundVector;
    if(init()){
        bool quit = false;
        SDL_Rect Kumaon_Rect=getRect(Kumaon);
        SDL_Rect Girnar_Rect=getRect(Girnar);
        SDL_Rect Nilgiri_Rect=getRect(Nilgiri);
        SDL_Rect Jwala_Rect=getRect(Jwalamukhi);
        SDL_Rect Himadri_Rect=getRect(Himadri);
        SDL_Rect lhc_Rect=getRect(LHC);
        SDL_Rect miscRect={0,0,8,8};
        SDL_Rect groundRect={0,0,8,8};
        SDL_Rect pathRect = {0,0,8,8};

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

        SDL_RenderClear(renderer);      

        SDL_Event e;

        while(!quit){
            while(SDL_PollEvent(&e)){
                if(e.key.keysym.sym == SDLK_ESCAPE){
                    quit= true;
                }
            }
            SDL_SetRenderDrawColor(renderer,0x00,0x00,0x00,0x00);
            SDL_RenderClear(renderer);
            for(int i=0;i<pathVector.size();i++){
                SDL_SetRenderDrawColor(renderer,0xF1,0x11,0xAA,SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer,&pathVector[i]);
            }

            for(int i=0;i<miscVector.size();i++){
                SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
                SDL_RenderFillRect(renderer,&miscVector[i]);
            }

            for(int i=0;i<groundVector.size();i++){
                SDL_SetRenderDrawColor(renderer,0x00,0xFF,0x00,0xFF);
                SDL_RenderFillRect(renderer,&groundVector[i]);
            }

            render("./Images/kumaon.jpeg",Kumaon_Rect);
            render("./Images/girnar.jpeg",Girnar_Rect);
            render("./Images/himadri.jpg",Himadri_Rect);
            render("./Images/nilgiri.jpg",Nilgiri_Rect);
            render("./Images/jwala.webp",Jwala_Rect);
            render("./Images/lhc.jpeg",lhc_Rect);

            SDL_RenderPresent(renderer);
        }
    }
    close();
    return 0;
}