#include <string>
#include <iostream>
#include <cmath>
#include <string.h>
// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

#include "game.h"
using namespace std;
int Game::keeppoints = 0 ;

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
    mvwprintw(this->mWindows[0], 6, 1, "Time Remaining: ");
}

void Game::renderInformationBoard() const
{ if (has_colors() == FALSE)
{
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
}

    start_color();			/* Start color 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);
    wattron(this->mWindows[0], COLOR_PAIR(1)| A_BOLD);
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "Author: Lei Mao");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/leimao/");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wattroff(this->mWindows[0],COLOR_PAIR(1)|A_BOLD);
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    wattron(this->mWindows[0], COLOR_PAIR(2)| A_BOLD);
    mvwprintw(this->mWindows[0], 1, 50, "        ");
    mvwprintw(this->mWindows[0], 2, 52, "  ");
    mvwprintw(this->mWindows[0], 3, 54, "  ");
    mvwprintw(this->mWindows[0], 4, 50, "        ");
    mvwprintw(this->mWindows[0], 1, 60, "    ");
    mvwprintw(this->mWindows[0], 1, 70, "  ");
    mvwprintw(this->mWindows[0], 2, 60, "  ");
    mvwprintw(this->mWindows[0], 2, 64, "  ");
     mvwprintw(this->mWindows[0], 2, 70, "  ");
    mvwprintw(this->mWindows[0], 3, 66, "  ");
    mvwprintw(this->mWindows[0], 3, 60, "  ");
    mvwprintw(this->mWindows[0], 3, 70, "  ");
    mvwprintw(this->mWindows[0], 4, 68, "    ");
    mvwprintw(this->mWindows[0], 4, 60, "  ");
    //A
    mvwprintw(this->mWindows[0], 1, 80, "  ");
    mvwprintw(this->mWindows[0], 2, 78, "  ");
    mvwprintw(this->mWindows[0], 2, 82, "  ");
    mvwprintw(this->mWindows[0], 3, 76, "          ");
    mvwprintw(this->mWindows[0], 4, 74, "  ");
    mvwprintw(this->mWindows[0], 4, 86, "  ");
    //K
    mvwprintw(this->mWindows[0], 1, 90, "  ");
    mvwprintw(this->mWindows[0], 1, 94, "  ");
    mvwprintw(this->mWindows[0], 2, 90, "    ");
    mvwprintw(this->mWindows[0], 3, 90, "    ");
    mvwprintw(this->mWindows[0], 4, 90, "  ");
    mvwprintw(this->mWindows[0], 4, 94, "  ");
    //E
    mvwprintw(this->mWindows[0], 1, 98, "      ");
    mvwprintw(this->mWindows[0], 2, 98, "      ");
    mvwprintw(this->mWindows[0], 3, 98, "  ");
    mvwprintw(this->mWindows[0], 4, 98, "      ");
    wattroff(this->mWindows[0], COLOR_PAIR(2)| A_BOLD);
    wrefresh(this->mWindows[0]);

}
//wattron


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

int Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
     std::vector<std::string> menuItems = {"Restart", "Quit","PAY $5 And Revive"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());
    mvwprintw(menu, 2 + offset, 1, menuItems[2].c_str());

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
        return 2;
    }
    else if(index==1)
    {
        return 0;
    }
    else if(index ==2)
    {
        return 1;
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
    Item mg(51, 7, magnet);
    mPtrSnake->mItems.push_back(mg);

}

void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->changeSnakeSymbol('@');
    this->initializeStatus();
    //this->createRamdonFood();
    mFood.reset(51,4);
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
{start_color();			/*color*/
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    wattron(this->mWindows[1], COLOR_PAIR(4)| A_BOLD);
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol);
    wrefresh(this->mWindows[1]);
    wattroff(this->mWindows[1], COLOR_PAIR(4)| A_BOLD);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
   std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();

    //新增加的蛇头显示
    Direction DIR = this->mPtrSnake->getDirection();

    if(this->mDifficulty >=1&&this->mDifficulty<=2){
//只有长度超过7才长出头
    start_color();			/*color*/
    init_pair(4, COLOR_RED, COLOR_BLACK);
    wattron(this->mWindows[1], COLOR_PAIR(4)| A_BOLD);

    if(DIR== Direction::Up){
    mvwaddch(this->mWindows[1], snake[0].getY()+1, snake[0].getX()+1, '>');
    mvwaddch(this->mWindows[1], snake[0].getY()+1, snake[0].getX()-1, '<');
    }
    if(DIR== Direction::Down){
    mvwaddch(this->mWindows[1], snake[0].getY()-1, snake[0].getX()+1, '>');
    mvwaddch(this->mWindows[1], snake[0].getY()-1, snake[0].getX()-1, '<');
    }
    if(DIR== Direction::Left){
    mvwprintw(this->mWindows[1], snake[0].getY()-1, snake[0].getX()+1, "/\\");
    mvwprintw(this->mWindows[1], snake[0].getY()+1, snake[0].getX()+1, "\\/");
    }
    if(DIR== Direction::Right){
    mvwprintw(this->mWindows[1], snake[0].getY()-1, snake[0].getX()-1, "/\\");
    mvwprintw(this->mWindows[1], snake[0].getY()+1, snake[0].getX()-1, "\\/");
    }
    wattroff(this->mWindows[1], COLOR_PAIR(4)| A_BOLD);


     start_color();			/*color*/
    init_pair(3, COLOR_RED, COLOR_BLUE);
    wattron(this->mWindows[1], COLOR_PAIR(3)| A_BOLD);
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);

    }
    wattroff(this->mWindows[1], COLOR_PAIR(3)| A_BOLD);}
    //end




    else if(this->mDifficulty >=3){//只有长度超过15才长出头
    start_color();			/*color*/
    init_pair(5, COLOR_BLUE, COLOR_RED);
    wattron(this->mWindows[1], COLOR_PAIR(5)| A_BOLD);

    if(DIR== Direction::Up){
    mvwaddch(this->mWindows[1], snake[0].getY()+1, snake[0].getX()+1, '>');
    mvwaddch(this->mWindows[1], snake[0].getY()+1, snake[0].getX()-1, '<');
    }
    if(DIR== Direction::Down){
    mvwaddch(this->mWindows[1], snake[0].getY()-1, snake[0].getX()+1, '>');
    mvwaddch(this->mWindows[1], snake[0].getY()-1, snake[0].getX()-1, '<');
    }
    if(DIR== Direction::Left){
    mvwprintw(this->mWindows[1], snake[0].getY()-1, snake[0].getX()+1, "/\\");
    mvwprintw(this->mWindows[1], snake[0].getY()+1, snake[0].getX()+1, "\\/");
    }
    if(DIR== Direction::Right){
    mvwprintw(this->mWindows[1], snake[0].getY()-1, snake[0].getX()-1, "/\\");
    mvwprintw(this->mWindows[1], snake[0].getY()+1, snake[0].getX()-1, "\\/");
    }
    wattroff(this->mWindows[1], COLOR_PAIR(5)| A_BOLD);


    for (int i = 0; i < snakeLength; i ++)
    {   int t;
        t = rand()%5;
        if(i!=0&&i!=1&&i!=2){
        start_color();			/*color*/
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(0, COLOR_YELLOW, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_WHITE, COLOR_BLACK);

        wattron(this->mWindows[1], COLOR_PAIR(t)| A_BOLD);}
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
        if(i!=0&&i!=1&&i!=2){
        wattroff(this->mWindows[1], COLOR_PAIR(t)| A_BOLD);
    }}}
    else
    {     start_color();			/*color*/
    init_pair(5, COLOR_BLUE, COLOR_RED);
    wattron(this->mWindows[1], COLOR_PAIR(5)| A_BOLD);
        for (int i = 0; i < snakeLength; i ++){
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol);
    }wattroff(this->mWindows[1], COLOR_PAIR(5)| A_BOLD);
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
    this->mPoints = this->keeppoints;

    while (lives > 0)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        attract_food();
        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision)
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


        if (eatFood)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
        }

        eatItem();
        influenceBychick();
        this->adjustDelay();



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
    int choice;
    while (true)
    {
        this->readLeaderBoard();
        this->renderBoards();
        this->initializeGame();
        this->runGame();
        this->updateLeaderBoard();
        this->writeLeaderBoard();
        choice = this->renderRestartMenu();
        if (choice == 0)
        {
            break;
        }
        else if(choice ==1){
            this->keeppoints = this->mPoints;
            this->revive();
        }
        else if(choice ==2){this->keeppoints =0;
        this->revive();}
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
    if (tp == chick) playSound(soundType::Chicken);
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
        case magnet: runMagnet();break;
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
    withMagnet = false;
    nearChick = false;
    isaccelerated = false;
    lives = 1;
    moreDifficulty = 0;
    runintoCorner = false;
}

void Game::runMagnet() {
    withMagnet = true;
    mCounter_magnet = 3;
    start_magnet = clock();
    //time_t startTime = time
}

void Game::attract_food() {
    if (withMagnet) {
        end_magnet = clock();
        duration =  (double )(end_magnet - start_magnet) / CLOCKS_PER_SEC;
        mCounter_magnet = ceil(3 - duration);
        if (mCounter_magnet > 0){
            foodMove();
        }else{
            withMagnet = false;
            mPtrSnake->mItems[4] = createRandomItem(magnet);
        }
    }
}

void Game::foodMove() {
    SnakeBody newhead = mPtrSnake->createNewHead();
    int distanceX = mFood.getX() - newhead.getX();
    int distanceY = mFood.getY() - newhead.getY();
    if (distanceX > 0) {
        if (distanceX > 1) mFood.reset(mFood.getX() - 2, mFood.getY());
        if (distanceX == 1) mFood.reset(mFood.getX() - 1, mFood.getY());
    }
    if (distanceX < 0){
        if (distanceX < -1) mFood.reset(mFood.getX() + 2, mFood.getY());
        if (distanceX == -1) mFood.reset(mFood.getX() + 1, mFood.getY());
    }
    if (distanceY > 0) {
        if (distanceY > 1) mFood.reset(mFood.getX(), mFood.getY() - 2);
        if (distanceY == 1) mFood.reset(mFood.getX() ,mFood.getY() - 1 );
    }
    if (distanceY < 0) {
        if (distanceY < -1) mFood.reset(mFood.getX(), mFood.getY() + 2);
        if (distanceY == -1) mFood.reset(mFood.getX() ,mFood.getY() + 1 );
    }
    mPtrSnake->mFood = mFood;
}

void Game::revive()
{
    this->mInitialSnakeLength =this->keeppoints+2;
}

// 启动奖励机制
void Game::startReward()
{
    if (this->mPtrSnake->getLength() % 5 == 0)
    {
        this->mRewardState = RewardState::Active;
        this->mRewardCountdown = 5;
        this->mRewardPoints = 10;
        this->adjustDelay();
    }
}

// 停止奖励机制
void Game::stopReward()
{
    this->mRewardState = RewardState::Inactive;
    this->mRewardCountdown = 0;
    this->mRewardPoints = 0;
    this->adjustDelay();
}

// 更新奖励倒计时
void Game::updateRewardCountdown()
{
    if (this->mRewardState == RewardState::Countdown)
    {
        this->mRewardCountdown--;
        // 当倒计时达到0时，停止奖励
        if (this->mRewardCountdown <= 0)
        {
            this->stopReward();
        }
        this->adjustDelay();
    }
    this->mRewardTimeRemaining = this->mRewardCountdown;
    this->updateRewardTimeRemaining();

}

// 处理奖励触发时的逻辑
void Game::processReward()
{
    if (this->mRewardState == RewardState::Active)
    {
        // 触发奖励时，增加蛇的长度
        for (int i = 0; i < this->mRewardPoints; i++)
        {
            this->mPtrSnake->moveFoward();
        }
        // 进入倒计时状态
        this->mRewardState = RewardState::Countdown;
    }
    else if (this->mRewardState == RewardState::Countdown)
    {
        // 在倒计时状态下，更新剩余时间并计算分数
        this->updateRewardCountdown();
        this->mPoints += 2;
        this->renderPoints();
        this->updateRewardTimeRemaining();

    }
}

// 更新倒计时剩余时间
void Game::updateRewardTimeRemaining()
{
    mvwprintw(this->mWindows[0], 6, 16, "     ");
    mvwprintw(this->mWindows[0], 6, 16, std::to_string(this->mRewardTimeRemaining).c_str());
    wrefresh(this->mWindows[0]);
}










