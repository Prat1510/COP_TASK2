#define ENET_IMPLEMENTATION
#include "enet.h"
#include <stdio.h>
#include "CoviWar.hpp"

int main()
{
    if (enet_initialize () != 0) {
        cout<<"An error occurred while initializing ENet."<<endl;
        return 1;
    }
    cout<<"here"<<endl;
    CoviWar game;
    cout<<"here"<<endl;
    game.run();
    enet_deinitialize();
    return 0;
}






