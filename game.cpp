#include "game.hpp"

int main()
{
    LCD.SetBackgroundColor(FEHLCD::White);
    LCD.Clear();

    LCD.SetFontColor(FEHLCD::Red);
    LCD.DrawPixel(175, 175);
    LCD.DrawHorizontalLine(25, 50, 200);
    LCD.DrawVerticalLine(200, 0, 100);
    LCD.DrawRectangle(0, 0, 50, 50);
    LCD.FillRectangle(50, 50, 50, 50);

    LCD.SetFontColor(FEHLCD::Green);
    LCD.FillRectangle(320 - 50, 240 - 50, 50, 50);
    LCD.DrawVerticalLine(320 / 2, 0, 240);

    LCD.SetFontColor(FEHLCD::Blue);
    LCD.DrawLine(25, 25, 100, 100);
    LCD.DrawLine(50, 75, 75, 50);
    LCD.DrawLine(60, 200, 70, 20);

    while (1)
    {
        LCD.Update();
    }

    return 0;
}