#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

using namespace std;

namespace image_type{
    enum{
        Kumaon,
        Girnar,
        count
    };
}


class Image{
    SDL_Surface* image_elements[image_type::count];
    SDL_Renderer* renderer_image;
    void load(string str,int index){
        SDL_Surface* tmpSurface = IMG_Load(str.c_str());
        if(tmpSurface == NULL){
            cout<<"could not image "<<str<<" "<<IMG_GetError()<<endl;
            exit(EXIT_FAILURE);
        }
        image_elements[index] = tmpSurface; 
    }

    public:
        Image(SDL_Renderer* renderer):renderer_image(renderer){
            int imgFlags =  IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags)){
                cout<<"could not load SDL_image "<<IMG_GetError()<<endl;
                exit(EXIT_FAILURE);
            }

            load("Images/kumaon.jpeg",image_type::Kumaon);
            load("Images/girnar.jpeg",image_type::Girnar);
        }

        SDL_Surface* get(int index){
            return image_elements[index];
        }

        void renderCopy(SDL_Texture* texture,SDL_Rect &src,SDL_Rect &dest){
            SDL_RenderCopy(renderer_image,texture,&src,&dest);
        }

        ~Image(){
            IMG_Quit();
        }
};