#include "main.hpp"
#include <iostream>
#include <stdio.h>

int main()
{
    LCD.SetBackgroundColor(FEHLCD::Green);
    LCD.Clear();

    LCD.SetFontColor(FEHLCD::Black);
    LCD.Write("Did you ever hear the tragedy of Darth Plagueis The Wise? I thought not. It's not a story the Jedi would tell you. It's a Sith legend. Darth Plagueis was a Dark Lord of the Sith, so powerful and so wise he could use the Force to influence the midichlorians to create life... He had such a knowledge of the dark side that he could even keep the ones he cared about");

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
    LCD.WriteAt(927.333, 100, 170);
    LCD.WriteRC(f, 2, 5);
    LCD.WriteRC(false, 10, 17);

    // Close button
    LCD.SetFontColor(FEHLCD::White);
    LCD.FillRectangle(320 - 25, 0, 25, 25);
    LCD.FillRectangle(320 - 100, 25, 100, 25);
    LCD.SetFontColor(FEHLCD::Red);
    LCD.DrawRectangle(320 - 25, 0, 25, 25);
    LCD.DrawLine(320 - 24, 0, 320, 24);
    LCD.DrawLine(320 - 24, 24, 320, 0);
    LCD.WriteAt("close ^^", 320 - 100, 30);

    float mouseX, mouseY;

    float sleepytime = 0.1;

    FEHFile *testfile = SD.FOpen("input_text.txt", "r+");
    FEHFile *testfile2 = SD.FOpen("output_text.txt", "w+");

    int a = 0, b = 0;
    bool touched = false;

    SD.FScanf(testfile, "%d %d", &a, &b);

    while (!(touched && mouseX > 320 - 25 && mouseY < 25))
    {
        touched = LCD.Touch(&mouseX, &mouseY);

        std::cout << touched << " X: " << mouseX << ", Y: " << mouseY << ", A: " << a << ", B: " << b << ", RAND: " << Random.RandInt() << std::endl;
        std::cout << "TIME (S): " << TimeNowSec() << ", TIME (MS): " << TimeNowMSec() << ", TIME (S DECIMAL): " << TimeNow() << std::endl;

        SD.FPrintf(testfile2, "%d: %.0f, %.0f\n", touched, mouseX, mouseY);

        Sleep(sleepytime);

        LCD.Update();
    }

    // SD.FClose(testfile);
    // SD.FClose(testfile2);
    SD.FCloseAll();

    return 0;
}