#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<memory>
#include "font.hpp"
#include "global.hpp"
#include "input.hpp"
using namespace std;

class Game{
    enum class game_state{
        title,
        start,
        play,
        pause,
        gameover
    };

    SDL_Window* gWindow;
    SDL_Renderer* renderer;
    game_state _state;
    Font fonts;
    
    unique_ptr<Input> inputs;

    int selected;
    int game_mode;
    //render text to screen
    void text(string str,int size,Point &p,RGB color);
    //game states functions
    void game_title();
    void game_start();
    void game_play();
    void game_pause();
    void game_over();

    bool init();

    bool exit_from_game(){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT :
                    return false;
                case SDL_KEYDOWN :
                    if(e.key.keysym.sym == SDLK_ESCAPE){
                        return false;
                    }
                    break;
                default:break;
            }
        }
        return true;
    }

    //wait for renderer to load
    inline void wait_(){
        static Uint32 pre_count;
        const double wait_time = 1000.0 / 60;
        const Uint32 wait_count = (wait_time + 0.5);
        if (pre_count) {
            const Uint32 now_count = SDL_GetTicks();
            const Uint32 interval = now_count - pre_count;
            if (interval < wait_count) {
                const Uint32 delay_time = wait_count - interval;
            SDL_Delay(delay_time);
        }
        }
        pre_count = SDL_GetTicks();
    }

    void close(){
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(gWindow);
        renderer = NULL;
        gWindow = NULL;

        SDL_Quit();
        IMG_Quit();
    }
    public:
        Game():
            gWindow(NULL),
            renderer(NULL),
            _state(game_state::title),
            selected(0),
            game_mode(0)
        {
            init();
            inputs = make_unique<Input>();
            // _state = game_state::start;
        }

        void play(){
            while(true){
                inputs->update();
                if(!exit_from_game()){
                    break;
                }
                
                switch(_state){
                    case game_state::start :
                        game_start();
                        break;
                    case game_state::title :
                        game_title();
                        break;
                    case game_state::play :
                        game_play();
                        break;
                    case game_state::pause :
                        game_pause();
                        break;
                    case game_state::gameover :
                        game_over();
                        break;
                    default: break;                    
                }
                SDL_RenderPresent(renderer);
                wait_();
            }
        }

        ~Game(){
            close();
        }
};