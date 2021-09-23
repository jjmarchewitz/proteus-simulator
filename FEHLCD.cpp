#include "FEHLCD.h"
#include "tigr.h"
#include <iostream>

#define CharHeight 34 // 2 * 17, the normal character height on the Proteus
#define CharWidth 12  // 2 * 24, the normal character width on the Proteus

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define WINDOW_WIDTH 640 // TODO: Consider changing the actual window width and height to have a border around the "screen"
#define WINDOW_HEIGHT 480

#define REFRESH_RATE 70 //Hz

bool initialized = false;

FEHLCD LCD;
Tigr *screen;

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

    screen = tigrWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Proteus Simulator", TIGR_AUTO);
    Clear();
}

bool FEHLCD::Touch(float *x_pos, float *y_pos)
{
}

bool FEHLCD::Touch(int *x_pos, int *y_pos)
{
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
    // Currently takes in a 24-bit color as input
    TPixel rgbValues = tigrRGB((char)(color >> 16), (char)(color >> 8), (char)color);
    tigrClear(screen, rgbValues);
    tigrUpdate(screen);
}

void FEHLCD::Clear()
{
    Clear(_backcolor);
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
    SetRegisterColorValues();
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
    SetRegisterColorValues();
}

/*************************
*   DRAWING FUNCTIONS    *
*************************/

// DrawPixel takes in Proteus coordinates up to (320, 240)
void FEHLCD::DrawPixel(int x, int y)
{
    // Force X and Y to be positive
    x = (x < 0) ? 0 : x;
    y = (y < 0) ? 0 : y;

    // Force X and Y to be on-screen
    x = x % _width;
    x = x % _height;

    // TODO: Move the creation of this TPixel into SetFontColor()
    TPixel color = tigrRGB((char)(_forecolor >> 16), (char)(_forecolor >> 8), (char)_forecolor);
    // Use tigrFill because a "pixel" to the student is really a 2x2 rectangle because the dimensions of the screen are doubled
    tigrFill(screen, x * 2, y * 2, 2, 2, color);
    // No tigrUpdate() here because it was painfully slow updating after every pixel.
}

void FEHLCD::DrawHorizontalLine(int y, int x1, int x2)
{
    for (int i = x1; i <= x2; i++)
    {
        DrawPixel(i, y);
    }
    tigrUpdate(screen);
}

void FEHLCD::DrawVerticalLine(int x, int y1, int y2)
{
    for (int i = y1; i <= y2; i++)
    {
        DrawPixel(x, i);
    }
    tigrUpdate(screen);
}

void FEHLCD::DrawLine(int x1, int y1, int x2, int y2)
{
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
    tigrUpdate(screen);
}

void FEHLCD::DrawCircle(int x0, int y0, int r)
{
}

void FEHLCD::FillCircle(int x0, int y0, int r)
{
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

int FEHLCD::abs(int)
{
}

void FEHLCD::_Clear()
{
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