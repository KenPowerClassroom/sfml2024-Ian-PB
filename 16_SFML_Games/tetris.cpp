#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int length = 20;
const int width = 10;

int field[length][width] = {0};

struct Point
{ int x, y; } a[4], b[4];

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
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= width || a[i].y >= length)
        {
            return 0;
        }
        else if (field[a[i].y][a[i].x])
        {
            return 0;
        }
    }

   return 1;
};


int tetris()
{
    srand(time(0));     

    RenderWindow window(VideoMode(320, 480), "The Game!");

    Texture tileTexture;
    Texture backgroundTexture;
    Texture frameTexture;

    tileTexture.loadFromFile("images/tetris/tiles.png");
    backgroundTexture.loadFromFile("images/tetris/background.png");
    frameTexture.loadFromFile("images/tetris/frame.png");

    Sprite tileSprite(tileTexture);
    Sprite backgroundSprite(backgroundTexture);
    Sprite frameSprite(frameTexture);

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

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed)
                if (event.key.code == Keyboard::Up)
                {
                    rotate = true;
                }
                else if (event.key.code == Keyboard::Left)
                {
                    speedX = -1;
                }
                else if (event.key.code == Keyboard::Right)
                {
                    speedX = 1;
                }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            delay = 0.05;
        }

        //// <- Move -> ////
        for (int i = 0; i < 4; i++)
        { 
            b[i] = a[i]; 
            a[i].x += speedX;
        }
        if (!check())
        {
            for (int i = 0; i < 4; i++)
            {
                a[i] = b[i];
            }
        }

        //////Rotate//////
        if (rotate)
        {
            Point p = a[1]; //center of rotation
            for (int i=0;i<4;i++)
            {
                int x = a[i].y-p.y;
                int y = a[i].x-p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check())
            {
                for (int i = 0; i < 4; i++)
                {
                    a[i] = b[i];
                }
            }
        }

    ///////Tick//////
        if (timer > delay)
        {
            for (int i=0;i<4;i++) 
            { 
                b[i] = a[i];
                a[i].y += 1;
            }

            if (!check())
            {
                for (int i = 0; i < 4; i++)
                {
                    field[b[i].y][b[i].x] = colorNum;
                }

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;

                for (int i=0;i<4;i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }    
            }

            timer = 0;
        }

    ///////check lines//////////
        int k = length - 1;
        for (int i = length - 1; i > 0; i--)
        {
            int count=0;
            for (int j = 0; j < width; j++)
            {
                if (field[i][j]) count++;
                {
                    field[k][j] = field[i][j];
                }
            }
            if (count < width)
            {
                k--;
            }
        }

        speedX = 0;
        rotate = 0;
        delay = 0.3;

    /////////draw//////////
    window.clear(Color::White);    
    window.draw(backgroundSprite);
          
    for (int i = 0; i < length; i++)
        for (int j = 0; j < width; j++)
        {
            if (field[i][j] == 0)
            {
                continue;
            }

            tileSprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
            tileSprite.setPosition(j * 18, i * 18);
            tileSprite.move(28, 31); //offset
            window.draw(tileSprite);
        }

        for (int i = 0; i < 4; i++)
        {
            tileSprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
            tileSprite.setPosition(a[i].x * 18, a[i].y * 18);
            tileSprite.move(28, 31); //offset
            window.draw(tileSprite);
        }

        window.draw(frameSprite);
        window.display();
    }

    return 0;
}
