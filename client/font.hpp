#pragma once
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

using namespace std;

namespace font_size{
    enum{
        x96,
        x48,
        x32,
        x16,
        x8,
        count
    };
}


class Font{
    TTF_Font* font[font_size::count];
    void loadFonts(string str,unsigned int size,unsigned int index){
        font[index] = TTF_OpenFont(str.c_str(),size);
        if(font[index] == NULL){
            cout<<"font in not loaded "<<str<<" "<<TTF_GetError()<<endl;
            exit(EXIT_FAILURE);
        }
    }

    public:
        Font(){
            if(TTF_Init() == -1){
                cout<<"Fonts are not initialized "<<TTF_GetError()<<endl;
                exit(EXIT_FAILURE);
            }
            loadFonts("./Fonts/ShortBaby-Mg2w.ttf",96,font_size::x96);
            loadFonts("./Fonts/ShortBaby-Mg2w.ttf",48,font_size::x48);
            loadFonts("./Fonts/ShortBaby-Mg2w.ttf",32,font_size::x32);
            loadFonts("./Fonts/ShortBaby-Mg2w.ttf",16,font_size::x16);
            loadFonts("./Fonts/ShortBaby-Mg2w.ttf",12,font_size::x8);
        }

        TTF_Font* get(unsigned int index){
            return font[index];
        }

        ~Font(){
            for (const auto &font_element : font) {
            TTF_CloseFont(font_element);
            }
            TTF_Quit();
        }
};