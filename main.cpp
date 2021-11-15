#include "main.hpp"
#include <iostream>

#define VELOCITY 2
#define FEH_WIDTH 36
#define FEH_HEIGHT 17
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SLEEP_TIME 0.01

void randomizeColor();

int main()
{
    LCD.SetBackgroundColor(FEHLCD::White);
    LCD.SetFontColor(FEHLCD::Red);
    LCD.Update();

    int x = Random.RandInt() % SCREEN_WIDTH;
    int y = Random.RandInt() % SCREEN_HEIGHT;

    int x_direction = 1; // 1 is positive, -1 is negative
    int y_direction = 1;

    while (true)
    {
        LCD.Clear();

        LCD.WriteAt("FEH", x - (FEH_WIDTH / 2), y - (FEH_HEIGHT / 2));

        // If the text is about to go off-screen in the x-direction
        if (x + (FEH_WIDTH / 2) > SCREEN_WIDTH || x - (FEH_WIDTH / 2) < 0)
        {
            // Flip the direction the text is traveling
            x_direction *= -1;
            randomizeColor();
        }

        // If the text is about to go off-screen in the y-direction
        if (y + (FEH_HEIGHT / 2) > SCREEN_HEIGHT || y - (FEH_HEIGHT / 2) < 0)
        {
            // Flip the direction the text is traveling
            y_direction *= -1;
            randomizeColor();
        }

        // Increment the position of the text
        x += x_direction * VELOCITY;
        y += y_direction * VELOCITY;

        Sleep(SLEEP_TIME);
        LCD.Update();
    }

    return 0;
}

void randomizeColor()
{
    // Get a random new color
    FEHLCD::FEHLCDColor newColor = static_cast<FEHLCD::FEHLCDColor>(Random.RandInt() % 6);

    // Prevent the color from being the same as the background
    if (newColor != FEHLCD::White)
    {
        LCD.SetFontColor(newColor);
    }
    else
    {
        randomizeColor();
    }
}