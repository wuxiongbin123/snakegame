#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDE


#include <windows.h>
#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")
#include <thread>
#include <string>

enum class soundType
{
    BGM ,
    Chicken ,
    //BaketBall ,
    CollisionChicken ,
    Opening ,
    Quit
};

void playSound(soundType tp);

void playBGM();

void BaketBallSound();

void ChickSound();

void OpeningSound();

void QuitSound();

void CollisionChickenSound();

#endif // MUSIC_H_INCLUDED
