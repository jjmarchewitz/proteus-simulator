#include "FEHLCD.h"
#include "tigr.h"
#include <iostream>

#define CharHeight 34 // 2 * 17, the normal character height on the Proteus
#define CharWidth 12  // 2 * 24, the normal character width on the Proteus

#define LCD_WIDTH 640  // 2 * 320, the normal width of the Proteus LCD
#define LCD_HEIGHT 480 // 2 * 240, the normal height of the Proteus LCD

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
    screen = tigrWindow(LCD_WIDTH, LCD_HEIGHT, "Test", TIGR_AUTO);
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
    TPixel rgbValues;
    switch (color)
    {
    case Black:
        rgbValues = tigrRGB((char)(BLACK >> 16), (char)(BLACK >> 8), (char)BLACK);
        break;
    case White:
        rgbValues = tigrRGB((char)(WHITE >> 16), (char)(WHITE >> 8), (char)WHITE);
        break;
    case Red:
        rgbValues = tigrRGB((char)(RED >> 16), (char)(RED >> 8), (char)RED);
        break;
    case Green:
        rgbValues = tigrRGB((char)(GREEN >> 16), (char)(GREEN >> 8), (char)GREEN);
        break;
    case Blue:
        rgbValues = tigrRGB((char)(BLUE >> 16), (char)(BLUE >> 8), (char)BLUE);
        break;
    case Scarlet:
        rgbValues = tigrRGB((char)(SCARLET >> 16), (char)(SCARLET >> 8), (char)SCARLET);
        break;
    case Gray:
        rgbValues = tigrRGB((char)(GRAY >> 16), (char)(GRAY >> 8), (char)GRAY);
        break;
    default:
        rgbValues = tigrRGB((char)(BLACK >> 16), (char)(BLACK >> 8), (char)BLACK);
    }

    tigrClear(screen, rgbValues);
    tigrUpdate(screen);
}

void FEHLCD::Clear(unsigned int color)
{
    FEHLCDColor castedColor = static_cast<FEHLCDColor>(color);
    Clear(castedColor);
}

void FEHLCD::Clear()
{
    Clear(Black);
}

void FEHLCD::SetFontColor(FEHLCDColor color)
{
    _forecolor = color;
}

void FEHLCD::SetFontColor(unsigned int color)
{
    FEHLCDColor castedColor = static_cast<FEHLCDColor>(color);
    SetFontColor(castedColor);
}

void FEHLCD::SetBackgroundColor(FEHLCDColor color)
{
    _backcolor = color;
}

void FEHLCD::SetBackgroundColor(unsigned int color)
{
    FEHLCDColor castedColor = static_cast<FEHLCDColor>(color);
    SetBackgroundColor(castedColor);
}

// Drawing Functions
void FEHLCD::DrawPixel(int x, int y)
{
}

void FEHLCD::DrawHorizontalLine(int y, int x1, int x2)
{
}

void FEHLCD::DrawVerticalLine(int x, int y1, int y2)
{
}

void FEHLCD::DrawLine(int x1, int y1, int x2, int y2)
{
}

void FEHLCD::DrawRectangle(int x, int y, int width, int height)
{
}

void FEHLCD::FillRectangle(int x, int y, int width, int height)
{
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
}

unsigned int FEHLCD::Convert24BitColorTo16Bit(unsigned int color)
{
}

unsigned int FEHLCD::ConvertRGBColorTo16Bit(unsigned char r, unsigned char g, unsigned char b)
{
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