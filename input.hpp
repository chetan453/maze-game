#pragma once

#include<iostream>
#include<SDL2/SDL_image.h>

namespace input_keys{
    enum{
        up,
        down,
        left,
        right,
        w,
        a,s,d,
        enter,space,f,
        count
    };
}

//player1 = 0,player2 = 1


class Input{
    bool press_key[input_keys::count][2];
    bool edge_key[input_keys::count][2];

    public:
        Input(){
            for(int i=0;i<2;i++){
                for(int j=0;j<input_keys::count;j++){
                    press_key[j][i] = false;
                    edge_key[j][i] = false;
                }
            }
        }

        inline void update() noexcept {
            int player_num = 2;
            bool new_key[input_keys::count][player_num];

            for(int i=0;i<2;i++){
                for(int j=0;j<input_keys::count;j++){
                    new_key[j][i] = false;
                }
            }

            const Uint8* state = SDL_GetKeyboardState(NULL);

            new_key[input_keys::up][0] = state[SDL_SCANCODE_UP] == SDL_PRESSED;
            new_key[input_keys::down][0] = state[SDL_SCANCODE_DOWN] == SDL_PRESSED;
            new_key[input_keys::left][0] = state[SDL_SCANCODE_LEFT] == SDL_PRESSED;
            new_key[input_keys::right][0] = state[SDL_SCANCODE_RIGHT] == SDL_PRESSED;

            new_key[input_keys::a][1] = state[SDL_SCANCODE_A] == SDL_PRESSED;
            new_key[input_keys::w][1] = state[SDL_SCANCODE_W] == SDL_PRESSED;
            new_key[input_keys::s][1] = state[SDL_SCANCODE_S] == SDL_PRESSED;
            new_key[input_keys::d][1] = state[SDL_SCANCODE_D] == SDL_PRESSED;

            new_key[input_keys::space][0] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
            new_key[input_keys::space][1] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
            new_key[input_keys::enter][0] = state[SDL_SCANCODE_RETURN] == SDL_PRESSED;
            new_key[input_keys::enter][1] = state[SDL_SCANCODE_RETURN] == SDL_PRESSED;
            new_key[input_keys::f][0] = state[SDL_SCANCODE_F] == SDL_PRESSED;
            new_key[input_keys::f][1] = state[SDL_SCANCODE_F] == SDL_PRESSED;

            for(int i=0;i<player_num;i++){
                for(int j=0;j<input_keys::count;j++){
                    edge_key[j][i] = !press_key[j][i] && new_key[j][i];
                    press_key[j][i] = new_key[j][i];                    
                }
            }

        }

        bool get_edge(int key_,int player){
            return edge_key[key_][player];
        }

        bool get_press(int key_,int player) const {
            return press_key[key_][player];
        }
};