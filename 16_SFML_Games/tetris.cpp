#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int length = 20;
const int width = 10;

int field[length][width] = {0};

struct Point
{
    int x, y;
};

const int MAX_TILES_PER_BLOCK = 4;

Point currentPosition[MAX_TILES_PER_BLOCK];
Point lastPosition[MAX_TILES_PER_BLOCK];

int figures[7][4] =
{
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5  // O
};

bool check()
{
    for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
    {
        if (currentPosition[i].x < 0 || currentPosition[i].x >= width || currentPosition[i].y >= length)
        {
            return 0;
        }
        else if (field[currentPosition[i].y][currentPosition[i].x])
        {
            return 0;
        }
    }

   return 1;
};

void move(int t_speedX)
{
    for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
    {
        lastPosition[i] = currentPosition[i];
        currentPosition[i].x += t_speedX;
    }
    if (!check())
    {
        for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
        {
            currentPosition[i] = lastPosition[i];
        }
    }
}

void rotation(bool t_rotate)
{
    if (t_rotate)
    {
        Point p = currentPosition[1]; //center of rotation

        for (int currentTile = 0; currentTile < MAX_TILES_PER_BLOCK; currentTile++)
        {
            int x = currentPosition[currentTile].y - p.y;
            int y = currentPosition[currentTile].x - p.x;
            currentPosition[currentTile].x = p.x - x;
            currentPosition[currentTile].y = p.y + y;
        }
        if (!check()) // If the position is occupied:
        {
            for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
            {
                // Send to last possible position
                currentPosition[i] = lastPosition[i];
            }
        }
    }
}

void inputManager(RenderWindow& t_window, bool& t_rotate, int& t_speedX, float& t_delay)
{
    Event event;
    while (t_window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            t_window.close();

        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Up)
            {
                t_rotate = true;
            }
            else if (event.key.code == Keyboard::Left)
            {
                t_speedX = -1; // Move Left
            }
            else if (event.key.code == Keyboard::Right)
            {
                t_speedX = 1; // Move Right
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            t_delay = 0.05;
        }
    }
}

void tick(float& t_timer, float t_delay, int& t_colorNum)
{
    if (t_timer > t_delay)
    {
        for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
        {
            lastPosition[i] = currentPosition[i];
            currentPosition[i].y += 1;
        }

        if (!check())
        {
            for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
            {
                field[lastPosition[i].y][lastPosition[i].x] = t_colorNum;
            }

            t_colorNum = 1 + rand() % 7;
            int n = rand() % 7;

            for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
            {
                currentPosition[i].x = figures[n][i] % 2;
                currentPosition[i].y = figures[n][i] / 2;
            }
        }

        t_timer = 0;
    }
}

void checkLines()
{
    int currentLine = length - 1;
    for (int i = length - 1; i > 0; i--)
    {
        int count = 0;
        for (int j = 0; j < width; j++)
        {
            if (field[i][j]) count++;
            {
                field[currentLine][j] = field[i][j];
            }
        }
        if (count < width)
        {
            currentLine--;
        }
    }
}

void draw(RenderWindow& t_window, int t_colorNum)
{
    // Setup each drawable object
    Texture tileTexture;
    Texture backgroundTexture;
    Texture frameTexture;

    tileTexture.loadFromFile("images/tetris/tiles.png");
    backgroundTexture.loadFromFile("images/tetris/background.png");
    frameTexture.loadFromFile("images/tetris/frame.png");

    Sprite tileSprite(tileTexture);
    Sprite backgroundSprite(backgroundTexture);
    Sprite frameSprite(frameTexture);

    t_window.clear(Color::White);
    t_window.draw(backgroundSprite);

    for (int xCoord = 0; xCoord < length; xCoord++)
    {
        for (int yCoord = 0; yCoord < width; yCoord++)
        {
            if (field[xCoord][yCoord] == 0)
            {
                continue;
            }

            tileSprite.setTextureRect(IntRect(field[xCoord][yCoord] * 18, 0, 18, 18));
            tileSprite.setPosition(yCoord * 18, xCoord * 18);
            tileSprite.move(28, 31); //offset
            t_window.draw(tileSprite);
        }
    }

    for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
    {
        tileSprite.setTextureRect(IntRect(t_colorNum * 18, 0, 18, 18));
        tileSprite.setPosition(currentPosition[i].x * 18, currentPosition[i].y * 18);
        tileSprite.move(28, 31); //offset
        t_window.draw(tileSprite);
    }

    t_window.draw(frameSprite);
    t_window.display();
}

int tetris()
{
    srand(time(0));     

    RenderWindow window(VideoMode(320, 480), "The Game!");


    int speedX = 0; 
    bool rotate = 0; 
    int colorNum = 1;
    float timer = 0;
    float delay = 0.3;

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        // Input manager
        inputManager(window, rotate, speedX, delay);

        //// <- Move -> ////
        move(speedX);

        //////Rotate//////
        rotation(rotate);

        ///////Tick//////
        tick(timer, delay, colorNum);

        ///////Check Lines//////////
        checkLines();

        // Reset values
        speedX = 0;
        rotate = 0;
        delay = 0.3;

        /////////draw//////////
        draw(window, colorNum);
    }

    return 0;
}
