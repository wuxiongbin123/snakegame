
#include "music.h"
using namespace std;

void playSound(soundType tp)
{
    switch(tp)
    {
    case soundType::BGM:
        {
            thread tid(playBGM);
            tid.detach();
        }break;
    case soundType::Chicken:
        {
            thread tid(ChickSound);
            tid.detach();
        }break;
        /*
    case soundType::Food:
        {
            thread tid(FoodSound);
            tid.detach();
        }break;*/
    case soundType::Opening:
        {
            OpeningSound();
        }break;
        /*
    case soundType::Quit:
        {
            thread tid(QuitSound);
            tid.join();
        }break;*/
    }
}

void playBGM()
{
/*
    char a[100];
    mciSendString("status music volume", a , 100, 0);
    int b = atoi(a);
    char s[100];
    sprintf(s, "setaudio music volume to %d", b * 0);
    mciSendString(s, 0, 0, 0);
    */

    char a[100];
     mciSendString(TEXT("open bgm.mp3 alias mysong"),
                       0, 0, 0);
        mciSendString(TEXT("play mysong wait"), 0, 0, 0);

            sprintf(a, TEXT("setaudio music volume to 0"));
            mciSendString(a, 0, 0, 0);


        while (true){


            mciSendString(TEXT("play mysong from 0 wait"), 0, 0, 0);

        }
}

void ChickSound()
{
         mciSendString(TEXT("open chicken.mp3 alias mysong"),
                       NULL, 0, NULL);
        mciSendString(TEXT("play mysong wait"), NULL, 0, NULL);

}

void OpeningSound()
{

         mciSendString(TEXT("open opening.mp3 alias mysong"),
                       NULL, 0, NULL);
        mciSendString(TEXT("play mysong wait"), NULL, 0, NULL);

}


