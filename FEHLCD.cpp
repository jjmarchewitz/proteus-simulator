#include "FEHLCD.h"
#include "tigr.h"
#include <iostream>

#define CharHeight 34 // 2 * 17, the normal character height on the Proteus
#define CharWidth 12  // 2 * 24, the normal character width on the Proteus

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define WINDOW_WIDTH LCD_WIDTH // TODO: Consider changing the actual window width and height to have a border around the "screen"
#define WINDOW_HEIGHT LCD_HEIGHT

#define REFRESH_RATE 70 //Hz

bool initialized = false;

FEHLCD LCD;
Tigr *screen;
int scale;

FEHLCD::FEHLCD()
{
    Initialize();
}

void FEHLCD::Initialize()
{
    if (initialized == false)
    {
        initialized = true;

        _Initialize();
    }
}

void FEHLCD::_Initialize()
{
    _width = LCD_WIDTH;
    _height = LCD_HEIGHT;
    _forecolor = WHITE;
    _backcolor = BLACK;

    screen = tigrWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Proteus Simulator", TIGR_AUTO & TIGR_RETINA);

    Clear();
}

bool FEHLCD::Touch(float *x_pos, float *y_pos)
{
    int x_int, y_int;
    bool touched = Touch(&x_int, &y_int);
    *x_pos = x_int;
    *y_pos = y_int;

    return touched;
}

bool FEHLCD::Touch(int *x_pos, int *y_pos)
{
    int mouseButton;
    tigrMouse(screen, x_pos, y_pos, &mouseButton);

    // TODO: Test multiple mouse buttons including MMB.
    // True if left mouse button is pressed.
    return mouseButton == 1;
}

void FEHLCD::ClearBuffer()
{
}

void FEHLCD::PrintImage(int x, int y)
{
}

void FEHLCD::PrintLogo(int x, int y)
{
}

void FEHLCD::SetOrientation(FEHLCDOrientation orientation)
{
    _orientation = orientation;
}

void FEHLCD::Clear(FEHLCDColor color)
{
    unsigned int htmlColor = ConvertFEHColorTo24Bit(color);
    Clear(htmlColor);
}

void FEHLCD::Clear(unsigned int color)
{
    SetBackgroundColor(color);
    _Clear();
}

void FEHLCD::Clear()
{
    _Clear();
}

void FEHLCD::Update()
{
    tigrUpdate(screen);
}

void FEHLCD::SetFontColor(FEHLCDColor color)
{
    unsigned int htmlColor = ConvertFEHColorTo24Bit(color);
    SetFontColor(htmlColor);
}

void FEHLCD::SetFontColor(unsigned int color)
{
    // Currently takes in a 24-bit color as input
    _forecolor = color;
}

void FEHLCD::SetBackgroundColor(FEHLCDColor color)
{
    unsigned int htmlColor = ConvertFEHColorTo24Bit(color);
    SetBackgroundColor(htmlColor);
}

void FEHLCD::SetBackgroundColor(unsigned int color)
{
    // Currently takes in a 24-bit color as input
    _backcolor = color;
}

void Swap(int &a, int &b)
{
    int c = a;
    a = b;
    b = c;
}

/*************************
*   DRAWING FUNCTIONS    *
*************************/

// DrawPixel takes in Proteus coordinates up to (320, 240)
void FEHLCD::DrawPixel(int x, int y)
{
    // Force X and Y to be positive
    while (x < 0)
    {
        x += _width;
    }

    while (y < 0)
    {
        y += _height;
    }

    // Force X and Y to be within the LCD boundaries
    x = x % _width;
    y = y % _height;

    // TODO: Move the creation of this TPixel into SetFontColor()
    TPixel color = tigrRGB((char)(_forecolor >> 16), (char)(_forecolor >> 8), (char)_forecolor);
    tigrPlot(screen, x, y, color);
}

void FEHLCD::DrawHorizontalLine(int y, int x1, int x2)
{
    for (int i = x1; i <= x2; i++)
    {
        DrawPixel(i, y);
    }
}

void FEHLCD::DrawVerticalLine(int x, int y1, int y2)
{
    for (int i = y1; i <= y2; i++)
    {
        DrawPixel(x, i);
    }
}

void FEHLCD::DrawLine(int x1, int y1, int x2, int y2)
{

    // Using a float to be more precise, will cast the end result to an int
    float slope = (y2 - y1) / (x2 - x1);

    int xVal, yVal, startX, endX, startY, endY;

    // Makes the lower of the two input X values be the startX, and the higher be the endX
    startX = (x1 <= x2) ? x1 : x2;
    endX = (x1 <= x2) ? x2 : x1;

    for (xVal = startX; xVal <= endX; xVal++)
    {
        // For lines with a slope > 1, there will sometimes be multiple Y values for the same X value after the pixels
        // are truncated when casted to ints. These variables and the loop below ensure that all pixels are plotted
        startY = (int)(slope * (xVal - x1) + y1);
        endY = (int)(slope * (xVal - x1 + 1) + y1);

        // Make sure to begin at the lower Y value
        if (endY < startY)
        {
            Swap(startY, endY);
        }

        // Draw all of the Y values for a given X value
        for (yVal = startY; yVal <= endY; yVal++)
        {
            DrawPixel(xVal, yVal);
        }
    }
}

void FEHLCD::DrawRectangle(int x, int y, int width, int height)
{
    DrawHorizontalLine(y, x, x + width);
    DrawHorizontalLine(y + height, x, x + width);
    DrawVerticalLine(x, y, y + height);
    DrawVerticalLine(x + width, y, y + height);
}

void FEHLCD::FillRectangle(int x, int y, int width, int height)
{
    for (int currentX = x; currentX <= x + width; currentX++)
    {
        for (int currentY = y; currentY <= y + height; currentY++)
        {
            DrawPixel(currentX, currentY);
        }
    }
}

void FEHLCD::DrawCircle(int x0, int y0, int r)
{
    // This alogorithm is from wikipedia
    // It's called the "midpoint circle algorithm"
    // or the "Bresenham's circle algorithm"
    // http://en.wikipedia.org/wiki/Midpoint_circle_algorithm
    // See the page for further details
    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    DrawPixel(x0, y0 + r);
    DrawPixel(x0, y0 - r);
    DrawPixel(x0 + r, y0);
    DrawPixel(x0 - r, y0);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        DrawPixel(x0 + x, y0 + y);
        DrawPixel(x0 - x, y0 + y);
        DrawPixel(x0 + x, y0 - y);
        DrawPixel(x0 - x, y0 - y);
        DrawPixel(x0 + y, y0 + x);
        DrawPixel(x0 - y, y0 + x);
        DrawPixel(x0 + y, y0 - x);
        DrawPixel(x0 - y, y0 - x);
    }
}

void FEHLCD::FillCircle(int x0, int y0, int r)
{
    // This algorithm is a variant on DrawCircle.
    // Rather than draw the points around the circle,
    // We connect them with a series of lines
    // to fill in the circle.

    int f = 1 - r;
    int ddF_x = 1;
    int ddF_y = -2 * r;
    int x = 0;
    int y = r;

    DrawVerticalLine(x0, y0 - r, y0 + r);
    DrawHorizontalLine(y0, x0 - r, x0 + r);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        DrawHorizontalLine(y0 + x, x0 - y, x0 + y);
        DrawHorizontalLine(y0 - x, x0 - y, x0 + y);
        DrawVerticalLine(x0 + x, y0 - y, y0 + y);
        DrawVerticalLine(x0 - x, y0 - y, y0 + y);
    }
}

// Write information at a specific Pixel on the screen
void FEHLCD::WriteAt(const char *str, int x, int y)
{
}

void FEHLCD::WriteAt(int i, int x, int y)
{
}

void FEHLCD::WriteAt(float f, int x, int y)
{
}

void FEHLCD::WriteAt(double d, int x, int y)
{
}

void FEHLCD::WriteAt(bool b, int x, int y)
{
}

void FEHLCD::WriteAt(char c, int x, int y)
{
}

// Write to Row, Column

void FEHLCD::WriteRC(const char *str, int row, int col)
{
}

void FEHLCD::WriteRC(int i, int row, int col)
{
}

void FEHLCD::WriteRC(float f, int row, int col)
{
}

void FEHLCD::WriteRC(double d, int row, int col)
{
}

void FEHLCD::WriteRC(bool b, int row, int col)
{
}

void FEHLCD::WriteRC(char c, int row, int col)
{
}

// Write to the screen
void FEHLCD::Write(const char *str)
{
}

void FEHLCD::Write(int i)
{
}

void FEHLCD::Write(float f)
{
}

void FEHLCD::Write(double d)
{
}

void FEHLCD::Write(bool b)
{
}

void FEHLCD::Write(char c)
{
}

// Write to the screeen and advance to next line
void FEHLCD::WriteLine(const char *str)
{
}

void FEHLCD::WriteLine(int i)
{
}

void FEHLCD::WriteLine(float f)
{
}

void FEHLCD::WriteLine(double d)
{
}

void FEHLCD::WriteLine(bool b)
{
}

void FEHLCD::WriteLine(char c)
{
}

typedef struct regColVal
{
    uint32_t BVal;
    uint32_t CVal;
    uint32_t DVal;
} RegisterColorValues;

void FEHLCD::TS_SPI_Init()
{
}

int FEHLCD::abs(int n)
{
    if (n < 0)
    {
        return -n;
    }
    else
    {
        return n;
    }
}

void FEHLCD::_Clear()
{
    // Currently takes in a 24-bit color as input
    TPixel rgbValues = tigrRGB((char)(_backcolor >> 16), (char)(_backcolor >> 8), (char)_backcolor);
    tigrClear(screen, rgbValues);
}

void FEHLCD::_RepeatColor()
{
}

void FEHLCD::_BackPixel()
{
}

void FEHLCD::_ForePixel()
{
}

void FEHLCD::SetRegisterColorValues()
{
}

void FEHLCD::WriteChar(int row, int col, char c)
{
}

void FEHLCD::WriteCharAt(int x, int y, char c)
{
}

void FEHLCD::WriteIndex(unsigned char index)
{
}

void FEHLCD::WriteParameter(unsigned char param)
{
}

void FEHLCD::RepeatColor()
{
}

unsigned int FEHLCD::ConvertFEHColorTo24Bit(FEHLCDColor color)
{
    unsigned int htmlColor;

    switch (color)
    {
    case Black:
        htmlColor = BLACK;
        break;
    case White:
        htmlColor = WHITE;
        break;
    case Red:
        htmlColor = RED;
        break;
    case Green:
        htmlColor = GREEN;
        break;
    case Blue:
        htmlColor = BLUE;
        break;
    case Scarlet:
        htmlColor = SCARLET;
        break;
    case Gray:
        htmlColor = GRAY;
        break;
    default:
        htmlColor = BLACK;
    }

    return htmlColor;
}

unsigned int FEHLCD::Convert24BitColorTo16Bit(unsigned int color)
{
    unsigned char r = (color & 0xFF0000u) >> 16;
    unsigned char g = (color & 0x00FF00u) >> 8;
    unsigned char b = (color & 0x0000FFu);
    return ConvertRGBColorTo16Bit(r, g, b);
}

unsigned int FEHLCD::ConvertRGBColorTo16Bit(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int ru = r >> 2;
    unsigned int gu = g >> 2;
    unsigned int bu = b >> 2;
    return (ru << 12) | (gu << 6) | bu;
}

void FEHLCD::NextLine()
{
}

void FEHLCD::CheckLine()
{
}

void FEHLCD::NextChar()
{
}

void FEHLCD::SetDrawRegion(int x, int y, int width, int height)
{
}

/*
    FEHIcon
*/
namespace FEHIcon
{
    Icon::Icon()
    {
    }

    void Icon::SetProperties(char name[20], int start_x, int start_y, int w, int h, unsigned int c, unsigned int tc)
    {
    }

    void Icon::Draw()
    {
    }

    void Icon::Select()
    {
    }

    void Icon::Deselect()
    {
    }

    int Icon::Pressed(float x, float y, int mode)
    {
    }

    int Icon::WhilePressed(float xi, float yi)
    {
    }

    void Icon::ChangeLabelString(const char new_label[20])
    {
    }

    void Icon::ChangeLabelFloat(float val)
    {
    }

    void Icon::ChangeLabelInt(int val)
    {
    }

    /* Function prototype for drawing an array of icons in a rows by cols array with top, bot, left, and right margins from edges of screen, labels for each icon from top left across each row to the bottom right, and color for the rectangle and the text color */
    void DrawIconArray(Icon icon[], int rows, int cols, int top, int bot, int left, int right, char labels[][20], unsigned int col, unsigned int txtcol)
    {
    }
} // ns FEHIcon