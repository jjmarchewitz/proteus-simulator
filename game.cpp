#include "game.hpp"

int main()
{

    LCD.SetFontColor(FEHLCD::Red);
    LCD.SetBackgroundColor(FEHLCD::White);
    LCD.Clear();

    LCD.DrawPixel(75, 75);

    LCD.DrawHorizontalLine(25, 50, 200);
    LCD.DrawVerticalLine(200, 0, 100);
    LCD.DrawRectangle(0, 0, 50, 50);
    LCD.FillRectangle(50, 50, 50, 50);

    LCD.SetFontColor(FEHLCD::Green);
    LCD.FillRectangle(320 - 50, 240 - 50, 50, 50);
    LCD.DrawVerticalLine(320 / 2, 0, 240);

    while (1)
    {
    }

    return 0;
}