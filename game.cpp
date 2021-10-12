#include "game.hpp"
#include <iostream>

int main()
{
    LCD.SetBackgroundColor(FEHLCD::Green);
    LCD.Clear();

    LCD.SetFontColor(FEHLCD::Black);
    LCD.Write("Did you ever hear the tragedy of Darth Plagueis The Wise? I thought not. It's not a story the Jedi would tell you. It’s a Sith legend. Darth Plagueis was a Dark Lord of the Sith, so powerful and so wise he could use the Force to influence the midichlorians to create life... He had such a knowledge of the dark side that he could even keep the ones he cared");

    LCD.SetFontColor(FEHLCD::Red);
    LCD.DrawPixel(175, 175);
    LCD.DrawRectangle(0, 0, 50, 50);
    LCD.FillRectangle(50, 50, 50, 50);
    LCD.FillRectangle(320 - 50, 240 - 50, 50, 50);
    LCD.DrawCircle(35, 230, 65);
    LCD.FillCircle(250, 105, 40);

    LCD.SetFontColor(FEHLCD::Blue);
    LCD.DrawHorizontalLine(25, 50, 200);
    LCD.DrawVerticalLine(200, 0, 100);
    LCD.DrawVerticalLine(320 / 2, 0, 240);
    LCD.DrawLine(25, 25, 100, 100);
    LCD.DrawLine(50, 75, 75, 50);
    LCD.DrawLine(60, 200, 70, 20);

    char f = 70;
    LCD.WriteAt(278, 100, 150);
    LCD.WriteAt(92783.333, 100, 170);
    LCD.WriteRC(f, 2, 5);
    LCD.WriteRC(false, 10, 17);

    float mouseX, mouseY;

    while (1)
    {
        bool touched = LCD.Touch(&mouseX, &mouseY);

        std::cout << touched << " X: " << mouseX << ", Y:" << mouseY << std::endl;

        LCD.Update();
    }

    return 0;
}