#pragma once

#include "enet.h"
#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<memory>
#include<vector>
#include<thread>
#include<chrono>
#include "font.hpp"
#include "global.hpp"
#include "input.hpp"
#include "player.hpp"
#include "image.hpp"
#include "task.hpp"
#include<ctime>

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
    unique_ptr<Player> p1;
    unique_ptr<Player> p2;
    unique_ptr<Image> images;

    unique_ptr<Task> t1;
    unique_ptr<Task> t2;
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

    time_t intiale;
    time_t present;

    int min;
    int sec;
    int prev_min;
    int prev_sec;
    bool testf;
    ENetAddress address;
    ENetHost* server;
    ENetEvent event;
    ENetPeer* peer1;
    bool init();

    void draw_game();
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
            address = {0};
            address.host = ENET_HOST_ANY; /* Bind the server to the default localhost.     */
            address.port = 8123; /* Bind the server to port 7777. */
            server = enet_host_create(&address, 3, 2, 0, 0);
            if (server == NULL) {
                printf("An error occurred while trying to create an ENet server host.\n");
                //return 1;
            }
            printf("Started a server...\n");
            min = 0;
            sec = 0;
            prev_min = 0;
            prev_sec = 0;
            inputs = make_unique<Input>();
            p1 = make_unique<Player>(renderer,images.get(),inputs.get(),player_type::p1);
            this_thread::sleep_for(chrono::nanoseconds(501));
            p2 = make_unique<Player>(renderer,images.get(),inputs.get(),player_type::p2);

            t1 = make_unique<Task>(p1.get());
            t2 = make_unique<Task>(p2.get());

        }

        int byteToInt(unsigned char* byte) {

            int n = 0;

            n = n + (byte[0] & 0x000000ff);
            n = n + ((byte[1] & 0x000000ff) << 8);
            n = n + ((byte[2] & 0x000000ff) << 16);
            n = n + ((byte[3] & 0x000000ff) << 24);


            return n;
        }

        void intToByte(int n, unsigned char* result) {

            result[0] = n & 0x000000ff;
            result[1] = (n & 0x0000ff00) >> 8;
            result[2] = (n & 0x00ff0000) >> 16;
            result[3] = (n & 0xff000000) >> 24; 
        }

        inline void decode(ENetPacket* packet){
            if(packet->data[0]=='P'){
            p2->set_pos((Point) {byteToInt(&packet->data[1]), byteToInt(&packet->data[5])}) ;
            // if(packet->data[9]=='1'){
            //     if(p2->power_up>0){
            //         Mix_PlayChannel(se_type::siren, sound_manager_->get_se(se_type::siren),0);
            //         //tobesent[9]='1';
            //         // p1->power_mode = 400;
            //         // p2->power_up--;
            //         }
            // }
            }
            else if(packet->data[0]=='B'){//pause
                _state = game_state::pause;
            }
            else if(packet->data[0]=='N'){//playing
                _state = game_state::play;
            }
        }

        void play(){
            while(true){
                inputs->update();
                while (enet_host_service(server, &event, 0) > 0) {
                    switch (event.type) {
                        case ENET_EVENT_TYPE_CONNECT:
                            printf("A new client connected from %x:%u.\n",  event.peer->address.host, event.peer->address.port);
                            /* Store any relevant client information here. */
                            //event.peer->data = "Client information";
                            peer1 = event.peer;
                            testf = true;
                            break;

                        case ENET_EVENT_TYPE_RECEIVE:
                            // //printf("A packet of length %lu containing %s was received from %s on channel %u.\n",
                            //         event.packet->dataLength,
                            //         event.packet->data,
                            //         event.peer->data,
                            //         event.channelID);
                            /* Clean up the packet now that we're done using it. */
                            decode(event.packet);
                            enet_packet_destroy (event.packet);
                            break;

                        case ENET_EVENT_TYPE_DISCONNECT:
                            printf("%s disconnected.\n", event.peer->data);
                            /* Reset the peer's client information. */
                            event.peer->data = NULL;
                            break;

                        case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                            printf("%s disconnected due to timeout.\n", event.peer->data);
                            /* Reset the peer's client information. */
                            event.peer->data = NULL;
                            break;

                        case ENET_EVENT_TYPE_NONE:
                            break;
                    }
                }

                if(!exit_from_game()){
                    enet_host_destroy(server);
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
                // cout<<"befor render present"<<endl;
                SDL_RenderPresent(renderer);
                wait_();
            }
        }

        ~Game(){
            close();
        }
};

void Game::draw_game(){
    SDL_SetRenderDrawColor(renderer,0x08,0x39,0x44,0xFF);
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

    p1->move(renderer,peer1,testf);
    p2->move(renderer,peer1,testf);

    Point kp = {150,10};
    text("Kumoaon",font_size::x32,kp,RGB{0x99,0x00,0x4C});
    kp = {370,10};
    text("Girnar",font_size::x32,kp,RGB{0x99,0x00,0x4C});
    kp = {10,10};
    text("Jwala",font_size::x32,kp,RGB{0x99,0x00,0x4C});
    kp = {10,400};
    text("Nilgiri",font_size::x32,kp,RGB{0x99,0x00,0x4C});
    kp = {760,10};
    text("Academic Area",font_size::x32,kp,RGB{0x99,0x00,0x4C});
    kp = {1000,440};
    text("LHC",font_size::x32,kp,RGB{0x99,0x00,0x4C});
    
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

    tmp = {1180,185,100,100};
    Point time_pos = {1180,185};
    text(to_string(sec/60)+" : "+to_string(sec%60),font_size::x16,time_pos,RGB{0x22,0xFF,0xFF});
    SDL_RenderDrawRect(renderer,&tmp);
    // SDL_DestroyTexture(tmp1);
}

bool Game::init(){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        cout<<"cannot initialiaze sdl "<<SDL_GetError()<<endl;
        success = false;
    }else{
        gWindow = SDL_CreateWindow("Server",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screen::width,screen::height,SDL_WINDOW_SHOWN);
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
    SDL_Color col_ = {color.r,color.g,color.b,255};
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
            string title_text = "Life at IITD";
            text(title_text,font_size::x96,title_pos,RGB{0x11,0x7A,0x65});
            switch(selected){
                case 0:{
                    SDL_SetRenderDrawColor(renderer,0xE8,0xF8,0xF5,0x00);
                    SDL_Rect tmp = {290,270,430,80};
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
                    SDL_Rect tmp = {290,370,430,80};
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
            Point kp = {300,120};
            text("Instructions",font_size::x96,kp,RGB{0x22,0x99,0x54});
            kp = {100,250};
            text("use W A S D to move Player1",font_size::x48,kp,RGB{0x04,0x0C,0x29});
            kp = {100,300};
            text("use Up Down Left Right to move Player2",font_size::x48,kp,RGB{0x04,0x0C,0x29});
            kp = {100,350};
            text("Complete the task",font_size::x48,kp,RGB{0x04,0x0C,0x29});
            kp = {100,400};
            text("player who has done most task will win the game",font_size::x48,kp,RGB{0x04,0x0C,0x29});
            kp = {100,600};
            text("Press F to go back",font_size::x48,kp,RGB{0x29,0x0D,0x04});
            if(inputs->get_edge(input_keys::f,0 ) || inputs->get_edge(input_keys::f,1) )
                game_mode = 0;

            break;
        }
        default: break;

    }
}

void Game::game_start(){
    present = intiale = time(0);
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
    _state = game_state::play;

}

void Game::game_play(){
    present = time(0);
    // min = prev_min + localtime(&present)->tm_min - localtime(&intiale)->tm_min;
    sec = prev_sec + (localtime(&present)->tm_hour - localtime(&intiale)->tm_hour)*3600 + (localtime(&present)->tm_min - localtime(&intiale)->tm_min)*60 + localtime(&present)->tm_sec - localtime(&intiale)->tm_sec;
    draw_game();

    if(t1->has_reached()){
        t1->task_completed();
        t1->add_new_task();
    }

    if(t2->has_reached()){
        t2->task_completed();
        t2->add_new_task();
    }

    if (inputs->get_edge(input_keys::space,player_type::p1)) {
        if(testf){ENetPacket * packet = enet_packet_create ("B", strlen ("B") + 1 ,ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send (peer1, 0, packet);}
        _state = game_state::pause;
    }

    if(sec >= 120){
        _state = game_state::gameover;
    }

}

void Game::game_pause(){

    draw_game();
    Point p = {300,300};
    text(" Paused ",font_size::x96,p,RGB{0xFF,0xFF,0xFF});

    if (inputs->get_edge( input_keys::space,player_type::p1)) {
      if(testf){ENetPacket * packet = enet_packet_create ("N", strlen ("N") + 1 ,ENET_PACKET_FLAG_RELIABLE);
      enet_peer_send (peer1, 0, packet);}
      _state = game_state::play;
    }


}

void Game::game_over(){
    SDL_SetRenderDrawColor(renderer,0xB2,0xBA,0xBB,0xFF);
    SDL_RenderClear(renderer);
    Point game_pos = {120,100};
    text("Game over",font_size::x96,game_pos,RGB{0x11,0x7A,0x65});
    Point result_pos = {320,300};
    if(t1->completed_size() == t2->completed_size()){
        text("Tied!!",font_size::x48,result_pos,RGB{0xCF,0xDB,0x16});
    }else if(t1->completed_size()>t2->completed_size()){
        text("Player1 wins!!",font_size::x48,result_pos,RGB{0xCF,0xDB,0x16});
    }else{
        text("Player2 wins!!",font_size::x48,result_pos,RGB{0xCF,0xDB,0x16});
    }
}