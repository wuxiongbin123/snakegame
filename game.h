#ifndef GAME_H
#define GAME_H

#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>

#include "snake.h"



class Game
{
public:
    Game();
    ~Game();
    
		void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;
    
		void createInstructionBoard();
    void renderInstructionBoard() const;
		
		void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;
    
		void renderBoards() const;
    
		void initializeGame();
    void runGame();
    void renderPoints() const;
    void renderDifficulty() const;
    
		void createRamdonFood();
    void renderFood() const;
    void renderItem() const;
    void renderSnake() const;
    void controlSnake() const;
    
		void startGame();
    bool renderRestartMenu() const;
    void adjustDelay();

    //Functions for Items.
    friend Item;
    bool isOccupied(int x, int y);
    void initializeItems();
    Item createRandomItem(type tp);
    void eatItem();
    void runItem(type tp);
    void runBasketball();
    void runCentre_parting();
    void runOverall();
    void runChick();
    void influenceBychick();
    void changeSnakeSymbol(char ch);
    void shoulderCharge();
    SnakeBody createLeftHead(SnakeBody neck);
    void initializeStatus();

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    const char mFoodSymbol = '#';
    float mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    //std::vector<Item> GItems;
    const int mNumLeaders = 3;

    //Items.
    bool withBasketball = false;
    bool withCentre_parting = false;
    bool withOverall = false;
    bool nearChick = false;
    bool isaccelerated = false;
    int lives = 1;
    int moreDifficulty = 0;
    bool runintoCorner = false;

};

#endif
