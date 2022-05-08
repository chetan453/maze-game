#include<iostream>
#include <utility>
#define ENET_IMPLEMENTATION
#include "game.hpp"
#include "enet.h"
using namespace std;

int main(){
    if (enet_initialize () != 0) {
        printf("An error occurred while initializing ENet.\n");
        return 1;
    }
    Game game;
    game.play();
    enet_deinitialize();
    exit(EXIT_SUCCESS);
}