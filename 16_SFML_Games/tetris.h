#pragma once

struct Point
{
    int x, y;
};
const int MAX_TILES_PER_BLOCK = 4;

Point currentPosition[MAX_TILES_PER_BLOCK];
Point lastPosition[MAX_TILES_PER_BLOCK];

const int length = 20;
const int width = 10;

int field[length][width] = { 0 };

bool check()
{
    for (int i = 0; i < MAX_TILES_PER_BLOCK; i++)
    {
        if (currentPosition[i].x < 0 || currentPosition[i].x >= width || currentPosition[i].y >= length)
        {
            return false;
        }
        else if (field[currentPosition[i].y][currentPosition[i].x])
        {
            return false;
        }
    }

    return true;
};