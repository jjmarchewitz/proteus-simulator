#include "FEHLCD.h"

#define CharHeight 17
#define CharWidth 12

bool initialized = false;

#define LCD_WIDTH 320
#define LCD_HEIGHT 240

#define REFRESH_RATE 70 //Hz

FEHLCD LCD;

FEHLCD::FEHLCD()
{
}

void FEHLCD::_Initialize()
{
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

void FEHLCD::Initialize()
{
}

void FEHLCD::SetOrientation(FEHLCDOrientation orientation)
{
}

void FEHLCD::Clear(FEHLCDColor color)
{
}

void FEHLCD::Clear(unsigned int color)
{
}

void FEHLCD::Clear()
{
}

void FEHLCD::SetFontColor(FEHLCDColor color)
{
}

void FEHLCD::SetFontColor(unsigned int color)
{
}

void FEHLCD::SetBackgroundColor(FEHLCDColor color)
{
}

void FEHLCD::SetBackgroundColor(unsigned int color)
{
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

void FEHLCD::_Initialize()
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