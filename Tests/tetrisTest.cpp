#include "pch.h"

#include "../16_SFML_Games/tetris.h"

TEST(Tetris, checkOutsideLeftBorder) 
{
	// Checking if out of the screen on the left
	currentPosition[0].x = -1;
	EXPECT_EQ(false, check()); 
}

TEST(Tetris, checkOutsideRightBorder)
{
	// Checking if out of the screen on the right
	currentPosition[0].x = width + 1;
	EXPECT_EQ(false, check());
}

TEST(Tetris, checkWithingBorders)
{
	// Check if it works when in bounds (x-axis)
	for (int i = 0; i < width; i++)
	{
		currentPosition[0].x = i;
		EXPECT_EQ(true, check());
	}
}

TEST(Tetris, checkOutsideBottomBorder)
{
	// Check that it isn't higher than the roof
	currentPosition[0].x = 3;
	currentPosition[0].x = length + 1;

	EXPECT_EQ(false, check());
}