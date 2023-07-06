#include <string>
#include <iostream>
#include <cmath> 

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm> 

#include "game.h"
using namespace std;
Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();
    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "Author: Lei Mao");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/leimao/");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board 
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}


void Game::initializeItems() {

    mPtrSnake->mItems.push_back(createRandomItem(chick));
    mPtrSnake->mItems.push_back(createRandomItem(basketball));
    mPtrSnake->mItems.push_back(createRandomItem(centre_parting));
    mPtrSnake->mItems.push_back(createRandomItem(overall));

}

void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->changeSnakeSymbol('@');
    this->initializeStatus();
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
    this->initializeItems();
    this->mDifficulty = 0;
    this->mPoints = 0;
    this->mDelay = this->mBaseDelay;
}

void Game::createRamdonFood()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
}

void Game::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }
    wrefresh(this->mWindows[1]);
}

void Game::controlSnake() const
{
    int key;
    key = getch();

    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            if (!withCentre_parting){
                this->mPtrSnake->changeDirection(Direction::Up);
            }else{
                mPtrSnake->changeDirection(Direction::Down);
            }

            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            if (!withCentre_parting){
                this->mPtrSnake->changeDirection(Direction::Down);
            }else{
                mPtrSnake->changeDirection(Direction::Up);
            }

            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            if (!withCentre_parting){
                this->mPtrSnake->changeDirection(Direction::Left);
            }else{
                mPtrSnake->changeDirection(Direction::Right);
            }

            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            if(!withCentre_parting){
                this->mPtrSnake->changeDirection(Direction::Right);
            }else{
                mPtrSnake->changeDirection(Direction::Left);
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5 + moreDifficulty;
    if (int(mPoints) % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

void Game::runGame()
{
    bool moveSuccess;
    int key;
    while (lives > 0)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);


        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            if (!withOverall) break;
            else{
                withOverall = false;
                mPtrSnake->mItems[3] = createRandomItem(overall);
                shoulderCharge();
                if (runintoCorner) break;
            }
        }
        this->renderSnake();

        if (eatFood == true)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
        }
        this->adjustDelay();
        eatItem();
        influenceBychick();
        this->renderFood();
        this->renderDifficulty();
        this->renderPoints();
        this->renderItem();
        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }
}

void Game::startGame()
{
    refresh();
    bool choice;
    while (true)
    {
        this->readLeaderBoard();
        this->renderBoards();
        this->initializeGame();
        this->runGame();
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        choice = this->renderRestartMenu();
        if (choice == false)
        {
            break;
        }
    }
}

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}
/*
 * Here is where I begin to write my codes.
 * I also revise some codes in the above codes.
 */

bool Game::isOccupied(int i, int j){
    for (Item it: mPtrSnake->mItems){
        if (it.getX() == i && it.getY() == j) return true;
    }return false;
}

Item Game::createRandomItem(type tp) {
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            if(this->mPtrSnake->isPartOfSnake(j, i) || isOccupied(j, i) || (i == mFood.getX() && j == mFood.getY()))
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }
    int random_idx = std::rand() % availableGrids.size();
    Item it(availableGrids[random_idx].getX(), availableGrids[random_idx].getY(), tp);
    return it;
}

void Game::renderItem() const {
    for (Item it : mPtrSnake->mItems){
        int y = it.getY();
        int x = it.getX();
        char symbol = it.getSymbol();
        mvwaddch(this->mWindows[1], y, x, symbol);
        wrefresh(this->mWindows[1]);
    }
}

void Game::influenceBychick() {
    Item ch = mPtrSnake->mItems[0];
    SnakeBody head = mPtrSnake->mSnake[0];

    if ((head.getX() <= ch.getX() + 3) && (head.getX() >= ch.getX() - 3 )&&(head.getY() <= ch.getY() + 3) && (head.getY() >= ch.getY() - 3) )
    {
        nearChick = true;
    }else  nearChick = false;
    if (nearChick ){
        if (!isaccelerated){
            isaccelerated = true;
            moreDifficulty++;
        }
    }else{
        if (isaccelerated) {
            moreDifficulty--;
            isaccelerated = false;
        }
    }
}

void Game::eatItem() {
    bool flag = mPtrSnake->eatItem();
    if (flag){
        //delete the item and create a new one as well as run the function of the item.
        SnakeBody Head = mPtrSnake->mSnake[0];
        for (int i = 0; i < mPtrSnake->mItems.size(); i++){
            Item it = mPtrSnake->mItems[i];
            if (it.getX() == Head.getX() && it.getY() == Head.getY()){
                mPtrSnake->mItems[i] = Item(-1, -1, it.getType());//eat the item up and dismiss it.
                runItem(it.getType());
                break;
            }
        }
    }
}



void Game::runItem(type tp){
    switch (tp) {
        case basketball:runBasketball();break;
        case centre_parting: runCentre_parting();break;
        case overall: runOverall();break;
        case chick: runChick();break;
    }
}

void Game::runBasketball() {
    withBasketball = true;
    moreDifficulty++;
}

/*
 * Without centre_parting, the snake will perform logically, however,
 * with it, the snake will lead to the opposite direction.
 * Additionally, the symbol of snake will change to '?' when it loses its direction.
 */
void Game::runCentre_parting() {
    mPtrSnake->mItems[2] = createRandomItem(centre_parting);
    if (withCentre_parting) {
        withCentre_parting = false;
        changeSnakeSymbol('@');
    }
    else {
        withCentre_parting = true;
        changeSnakeSymbol('?');
    }
}

void Game::changeSnakeSymbol(char ch) {
    mSnakeSymbol = ch;
}

void Game::runOverall(){
    withOverall = true;
}

/*
 * ShoulderCharge, which is also named "Tieshankao" in Chinese,
 * can change the direction and avoid the snake dying from collide into the wall.
 */
void Game::shoulderCharge() {

    if (mPtrSnake->hitWall()) {
        SnakeBody head = mPtrSnake->mSnake[0];
        SnakeBody leftHead = createLeftHead(mPtrSnake->mSnake[1]);
        if (leftHead.getX() != 0 && leftHead.getX() != (mScreenWidth - 1)&&
        leftHead.getY() != 0 && leftHead.getY() != (mScreenHeight - 1)){
            mPtrSnake->mSnake.erase(std::remove(mPtrSnake->mSnake.begin(),
                                                mPtrSnake->mSnake.end(),head), mPtrSnake->mSnake.end());
            mPtrSnake->mSnake.insert(mPtrSnake->mSnake.begin(), leftHead);
            switch (mPtrSnake->mDirection) {
                case Direction::Up: mPtrSnake->changeDirection(Direction::Left);
                    break;
                case Direction::Down: mPtrSnake->changeDirection(Direction::Right);
                    break;
                case Direction::Left: mPtrSnake->changeDirection(Direction::Down);
                    break;
                case Direction::Right: mPtrSnake->changeDirection(Direction::Up);
                    break;
            }
        }else runintoCorner = true;
    }
}

SnakeBody Game::createLeftHead(SnakeBody neck) {
    int nx = neck.getX(), ny = neck.getY();
    int hx , hy;
    switch (mPtrSnake->mDirection) {
        case Direction::Up:{
            hx = nx - 1;
            hy = ny;
        }break;
        case Direction::Down:{
            hx = nx + 1;
            hy = ny;
        }break;
        case Direction::Left:{
            hx = nx ;
            hy = ny + 1;
        }break;
        case Direction::Right:{
            hx = nx ;
            hy = ny - 1;
        }break;
    }
    return SnakeBody(hx, hy);
}

void Game::runChick() {
    if (!withBasketball) lives--;
    else{
        withBasketball = false;
        moreDifficulty--;
        mPtrSnake->mItems[1] = createRandomItem(basketball);
        mPoints += 2.5;
        mPtrSnake->mItems[0] = createRandomItem(chick);
    }
}

void Game::initializeStatus() {
    changeSnakeSymbol('@');
    withBasketball = false;
    withCentre_parting = false;
    withOverall = false;
    nearChick = false;
    isaccelerated = false;
    lives = 1;
    moreDifficulty = 0;
    runintoCorner = false;
}












