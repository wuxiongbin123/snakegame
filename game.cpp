#include <string.h>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>

#include "game.h"
 int Game::keeppoints = 0 ;
 int Game::shut = 0;
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
{   if (has_colors() == FALSE)
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
    mvwprintw(this->mWindows[0], 2, 98, "  ");
    mvwprintw(this->mWindows[0], 2, 102, "  ");
    mvwprintw(this->mWindows[0], 3, 98, "    ");
    mvwprintw(this->mWindows[0], 4, 98, "       ");
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

int Game::renderRestartMenu()
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
//restart 2 exit 0 pay 1
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

void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength));
    this->createRamdonFood();
    this->mPtrSnake->senseFood(this->mFood);
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
{   start_color();			/*color*/
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

void Game::controlSnake()
{
    int key;
    key = getch();
    switch(key)
    {
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            break;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            break;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            break;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            break;
        }
            case 'H':
            case'h':
                {
                    this->shut++;

                    this->help();
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
    this->mDifficulty = this->mPoints / 5;
    if (mPoints % 5 == 0)
    {
        this->mDelay = this->mBaseDelay * pow(0.75, this->mDifficulty);
    }
}

void Game::runGame()
{
    bool moveSuccess;
    int key;
    this->mPoints = this->keeppoints;

    while (true)
    {
        this->controlSnake();
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);
        if(shut%2==0){

        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            break;
        }
        this->renderSnake();
        if (eatFood == true)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            this->adjustDelay();
        }
        this->renderFood();
        this->renderDifficulty();
        this->renderPoints();

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
    }}
}

void Game::startGame()
{
    refresh();
    int choice;//改变choice的类型，多加选项
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
//改动再render 上并将rungame 和bool menu 类型改为int

void Game::revive()
{
    this->mInitialSnakeLength =this->keeppoints+2;
}
void Game::shutup()
{
    int key;
    key = getch();
    if (key == 'H'||key =='h')
    {
    shut++;}
}

void Game::help()
{
    WINDOW * help;
    int width = this->mGameBoardWidth * 1;
    int height = this->mGameBoardHeight * 1;
    int startX = this->mGameBoardWidth * 0;
    int startY = this->mGameBoardHeight * 0+ this->mInformationHeight;

    help = newwin(height, width, startY, startX);
    box(help, 0, 0);
    int index = 0;
    int offset = 4;


mvwprintw(help, 3, 1, " 1.1 小 鸡 仔（  Q ） 擅 长 唱 跳 rap， 当 蛇 在 其 周 围 的 九 宫 格 之 内 ， 蛇 会 因 为 小 鸡 仔 的 rap 而 热 血 沸 腾 。 ");
mvwprintw(help, 4, 1, "   提 高 速 度 ， 当 离 开 影 响 范 围 时 ， 会 恢 复 原 来 的 速 度");
mvwprintw(help, 5, 1, "1.2 小 鸡 仔 爱 好 是 打 篮 球，如 果 蛇 经 过 时 携 带 篮 球，小 鸡 仔 会 拿 走 篮 球，并 加 上 2.5 分 作 为 酬 劳");
mvwprintw(help, 6, 1, "   但 是 如 果 没 有 携 带 篮 球 ( O )，直 面 小 鸡 仔 时 就 会 爆 炸");
mvwprintw(help, 7, 1, "1.3 中 分 （ ^ )：因 过 于 飘 逸，蛇 在 移 动 过 程 中 会 晕 头 转 向，因 此 在（吃 到 下 一 个 中 分 之 前 / 计 时 3s 之 内");
mvwprintw(help, 8, 1, "   蛇 的 运 动 方 向 会 改 变，并 且 蛇 的 全 身 会 变 成 ? ");
mvwprintw(help, 9 , 1 ,"1.4 背 带 裤(&) ：当 蛇 头 撞 到 墙 壁 时，会 触 发 铁 山 靠 技 能 ，脱 落 左 肩 的 吊 带，更 改 移 动 方 向 为 左 边。");
mvwprintw(help,10,1,"吸铁石 （ * ） 会 五 秒 内 吸 引 食 物");
mvwprintw(help,11,1 ,"2.1 奖 励 ：每 当 蛇 的 长 度 到 达 6 的 倍 数 时 ，触 发 奖 励 机 制。奖 励 倒 计 时 8s，初 始 奖 励 值 为 24，倒 计 时 每 过 1s，奖 励 减 3");
mvwprintw(help,12,1,"   而 8 s 后 奖 励 消 失 。倒 计 时 将 实 时 显 示");
mvwprintw(help, 13, 30, "⠀⠀⠀⠀⠰⢷⢿⠄");
mvwprintw(help, 14, 30, "⠀⠀⠀⠀⠀⣼⣷⣄");
mvwprintw(help, 15, 30, "⠀⠀⣤⣿⣇⣿⣿⣧⣿⡄");
mvwprintw(help, 16, 30, "⢴⠾⠋⠀⠀⠻⣿⣷⣿⣿⡀");
mvwprintw(help, 17, 30, "O  ⠀⢀⣿⣿⡿⢿⠈⣿");
mvwprintw(help, 18, 30, "⠀⠀⠀⢠⣿⡿⠁⠀⡊⠀⠙");
mvwprintw(help, 19, 30, "⠀⠀⠀⢿⣿⠀⠀⠹⣿");
mvwprintw(help, 20, 30, "⠀⠀⠀⢿⣿⠀⠀⠹⣿");
mvwprintw(help, 21, 30, "⠀⠀⠀⠀⠹⣷⡀⠀⣿⡄ 。");
mvwprintw(help, 22, 30, "⠀⠀⠀⠀⣀⣼⣿⠀⢈⣧ ");

    this->shutup();
    wrefresh(help);

}


