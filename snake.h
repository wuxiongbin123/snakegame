#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

class Game;
enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody& snakeBody);
    void reset(int newX, int newY);
private:
    int mX;
    int mY;
};
enum type{basketball, chick,  centre_parting, overall, magnet};

class Item{
private:
    int mX, mY;
    type tp;
    char symbol;
public:
    Item();
    Item(int x, int y, type t);
    int getX() const;
    int getY() const;
    int getSymbol() const;
    type getType() const;
};




// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    bool hitSelf();
    bool checkCollision();
    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    bool moveFoward();
    int getBoardWidth() const{ return mGameBoardHeight;}
    int getBoardHeight() const{ return mGameBoardHeight;}
    friend Item;
    friend Game;
    bool isOccupied(int x, int y);
    bool eatItem();
     Direction getDirection();

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;
    Direction mDirection;
    SnakeBody mFood;

    std::vector<SnakeBody> mSnake;
    std::vector<Item> mItems;
};

#endif
