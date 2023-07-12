#include <iostream>
#include <thread>

#include "game.h"

using namespace std;



int main()
{

    playSound(soundType::Opening);
    playSound(soundType::BGM);
   Game game;
   game.startGame();

    return 0;
}


