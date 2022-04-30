#include "FEHDashboard.h"
#include <iostream>
#include "FEHIO.h"

#define WINDOW_WIDTH 1320 // 880 // Even multiple of 11 (the number of columns) while being close to the aspect ratio described above
#define WINDOW_HEIGHT 700 // 500

#define CHAR_HEIGHT 7
#define CHAR_WIDTH 5

#define ROWS 4
#define COLUMNS 11
#define DIVIDER_SIZE 2
#define GENERAL_TEXT_Y_OFFSET 3 // The text offset in the X direction relative to the top left of each cell
#define MOTOR_TEXT_X_OFFSET 22
#define SERVO_TEXT_X_OFFSET 22
#define FEHIO_PIN_TEXT_X_OFFSET 34 // The text offset specifically for pin numbers like P1_7, because they are narrower than the whole cell
#define CELL_WIDTH WINDOW_WIDTH / COLUMNS
#define CELL_HEIGHT WINDOW_HEIGHT / ROWS

bool dashInitialized = false;
int _MouseButtons, _DummyX, _DummyY;
bool _ClickState[2] = {false, false};
FEHDashboard Dashboard;
FEHMutex dashMutex;

// Set up color constants. The graphics library we are using requires reference pixels to set the color
// of another pixel, rectangle, the whole screen, etc. These pixels act as that reference pixel to set
// the color of whatever is needed.
const TPixel FEHDashboard::BoolTrueColor = tigrRGB((char)(LIMEGREEN >> 16), (char)(LIMEGREEN >> 8), (char)LIMEGREEN);
const TPixel FEHDashboard::BoolFalseColor = tigrRGB((char)(RED >> 16), (char)(RED >> 8), (char)RED);
const TPixel FEHDashboard::BlackColor = tigrRGB((char)(BLACK >> 16), (char)(BLACK >> 8), (char)BLACK);
const TPixel FEHDashboard::WhiteColor = tigrRGB((char)(WHITE >> 16), (char)(WHITE >> 8), (char)WHITE);
const TPixel FEHDashboard::ScarletColor = tigrRGB((char)(SCARLET >> 16), (char)(SCARLET >> 8), (char)SCARLET);
const TPixel FEHDashboard::GreyColor = tigrRGB((char)(GRAY >> 16), (char)(GRAY >> 8), (char)GRAY);
const TPixel FEHDashboard::BackgroundColor = tigrRGB((char)(DARKGRAY >> 16), (char)(DARKGRAY >> 8), (char)DARKGRAY);
const TPixel FEHDashboard::RedColor = tigrRGB((char)(SCARLET >> 16), (char)(SCARLET >> 8), (char)SCARLET);
const TPixel FEHDashboard::GreenColor = tigrRGB((char)(FORESTGREEN >> 16), (char)(FORESTGREEN >> 8), (char)FORESTGREEN);
const TPixel FEHDashboard::BlueColor = tigrRGB((char)(ROYALBLUE >> 16), (char)(ROYALBLUE >> 8), (char)ROYALBLUE);

FEHDashboard::FEHDashboard()
{
    // TODO: Make sure that the dashboard only initializes if a dashboard/motor/servo/etc. function is called
}

void FEHDashboard::_Initialize()
{
    if (dashInitialized == false)
    {
        // TODO: Test TIGR_FIXED vs TIGR_AUTO vs TIGR_RETINA and combinations of those
        _Screen = tigrWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Proteus Simulator Dashboard", TIGR_FIXED & TIGR_RETINA);
        dashInitialized = true;
    }
}

void FEHDashboard::AddComponent(Component *comp)
{
    _Components.push_back(comp);
}

void FEHDashboard::Update()
{
    if (dashInitialized)
    {
        _ClearDash();
        _DrawUIBackground();

        // Loops through and draws each component
        for (auto comp : _Components)
        {
            comp->Draw();
        }

        dashMutex.lock();
        tigrUpdate(_Screen);
        dashMutex.unlock();

        // Update the mouse state array
        _ClickState[1] = _ClickState[0];
        tigrMouse(_Screen, &_DummyX, &_DummyY, &_MouseButtons);
        _ClickState[0] = (_MouseButtons & 0x1) == 0x1;
    }
}

bool FEHDashboard::MouseWasJustClicked()
{
    // Return true only if the mouse used to be unclicked but just now was clicked
    // std::cout << _ClickState[0] << ":" << _ClickState[1] << std::endl;
    return _ClickState[0] == true && _ClickState[1] == false;
}

void FEHDashboard::_ClearDash()
{
    tigrClear(_Screen, BackgroundColor);
}

void FEHDashboard::_DrawUIBackground()
{
    int currentColumnX, currentRowY;
    dashMutex.lock();

    // Draw the vertical bars. There should be 1 more bar than the number of columns to cover both edges of the screen
    for (int i = 0; i < COLUMNS + 1; i++)
    {
        currentColumnX = (CELL_WIDTH * i) - (DIVIDER_SIZE / 2);
        tigrFill(_Screen, currentColumnX, 0, DIVIDER_SIZE, WINDOW_HEIGHT, BlackColor);
    }

    // Draw the horizontal bars. There should be 1 more bar than the number of rows to cover both edges of the screen
    for (int i = 0; i < ROWS + 1; i++)
    {
        currentRowY = (CELL_HEIGHT * i) - (DIVIDER_SIZE / 2);
        tigrFill(_Screen, 0, currentRowY, WINDOW_WIDTH, DIVIDER_SIZE, BlackColor);
    }

    // The mutex is unlocked early because WriteAt() has its own mutex lock in place.
    // Trying to grab the lock twice crashes the program with a seg fault. I learned
    // the hard way :D
    dashMutex.unlock();

    // Draw the motor labels
    for (int i = 0; i < 4; i++)
    {
        currentRowY = (CELL_HEIGHT * i) + GENERAL_TEXT_Y_OFFSET;

        std::string motorLabel = "Motor" + std::to_string(i);
        WriteAt(motorLabel.c_str(), MOTOR_TEXT_X_OFFSET, currentRowY, RedColor, 2);
    }

    // Draw the servo labels
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
        {
            currentColumnX = CELL_WIDTH + SERVO_TEXT_X_OFFSET;
        }
        else
        {
            currentColumnX = 2 * CELL_WIDTH + SERVO_TEXT_X_OFFSET;
        }

        currentRowY = (CELL_HEIGHT * (i % 4)) + GENERAL_TEXT_Y_OFFSET;

        std::string servoLabel = "Servo" + std::to_string(i);
        WriteAt(servoLabel.c_str(), currentColumnX, currentRowY, GreenColor, 2);
    }

    // Draw the FEHIO port labels
    for (int portBank = 0; portBank < 4; portBank++)
    {
        // Sets the current row based on the bank number
        currentRowY = portBank * CELL_HEIGHT + GENERAL_TEXT_Y_OFFSET;

        for (int portNum = 0; portNum < 8; portNum++)
        {
            // The FEHIO ports start in the 4th column, so the first 3 cells need to be skipped (hence the)
            // "3 * CELL_WIDTH" at the beginning. The rest is making sure the current column is offset to
            // match the current portNum. Add 12 to make the text centered
            currentColumnX = 3 * CELL_WIDTH + portNum * CELL_WIDTH + FEHIO_PIN_TEXT_X_OFFSET;

            // Constructing the label of the form "P0_0" for each port
            std::string portLabel = "P" + std::to_string(portBank) + "_" + std::to_string(portNum);
            WriteAt(portLabel.c_str(), currentColumnX, currentRowY, BlueColor, 2);
        }
    }
}

void FEHDashboard::GetCellDimensions(int *width, int *height)
{
    *width = CELL_WIDTH;
    *height = CELL_HEIGHT;
}

// Write information at a specific Pixel on the screen
void FEHDashboard::WriteAt(const char *str, int x, int y, TPixel color, int pixelSize)
{
    int i = 0;
    while (str[i] != '\0')
    {
        WriteCharAt(x, y, str[i], color, pixelSize);
        x += (CHAR_WIDTH + 1) * pixelSize; // This controls the spacing between letters, the "+1" is the blank space
        i++;
    }
}

void FEHDashboard::WriteAt(int i, int x, int y, TPixel color, int pixelSize)
{
    char num[50];
    sprintf(num, "%d", i);
    WriteAt(num, x, y, color, pixelSize);
}

void FEHDashboard::WriteAt(float f, int x, int y, TPixel color, int pixelSize)
{
    char num[50];
    int d, r;
    if (f >= 0)
    {
        d = (int)f;
        r = (int)((f - d) * 1000);
        sprintf(num, "%d.%03d", d, r);
    }
    else
    {
        f *= -1;
        d = (int)f;
        r = (int)((f - d) * 1000);
        sprintf(num, "-%d.%03d", d, r);
    }
    WriteAt(num, x, y, color, pixelSize);
}

void FEHDashboard::WriteAt(double d, int x, int y, TPixel color, int pixelSize)
{
    WriteAt((float)d, x, y, color, pixelSize);
}

void FEHDashboard::WriteAt(bool b, int x, int y, TPixel color, int pixelSize)
{
    if (b)
    {
        WriteAt("true", x, y, color, pixelSize);
    }
    else
    {
        WriteAt("false", x, y, color, pixelSize);
    }
}

void FEHDashboard::WriteCharAt(int x, int y, char c, TPixel color, int pixelSize)
{
    // Force X and Y to be positive
    while (x < 0)
    {
        x += WINDOW_WIDTH;
    }

    while (y < 0)
    {
        y += WINDOW_HEIGHT;
    }

    // Force X and Y to be within the LCD boundaries
    x = x % WINDOW_WIDTH;
    y = y % WINDOW_HEIGHT;

    // Force the input character to be within the supported character list. c = 32 is a space.
    if (c > 125 || c < 32)
    {
        c = 32;
    }

    // Shift all of the input character values down by 32 so that they can be used as indexes of the fontData array
    c -= 32;

    // Look up the bitmap data out of the fontData table
    // Each entry in the fontData table corresponds
    // to a column of pixels in the 5x7 bitmapped character
    unsigned char charData[5];
    charData[0] = FEHLCD::fontData[5 * c];
    charData[1] = FEHLCD::fontData[5 * c + 1];
    charData[2] = FEHLCD::fontData[5 * c + 2];
    charData[3] = FEHLCD::fontData[5 * c + 3];
    charData[4] = FEHLCD::fontData[5 * c + 4];

    // Plot the character data pixel-by-pixel. Each character will be doubled in size from the fontData input,
    // becoming 10x14 instead of 5x7. There will be 2 additional columns of blank space to the left of each character
    // and 3 additional rows of blank space below, making each character 12x17 pixels.

    // Add blank columns
    x += pixelSize;

    dashMutex.lock();
    for (int col = 0; col < 5; col++)
    {
        for (int row = 0; row < 7; row++)
        {
            // If the current pixel is a 1 in the fontData bitmap
            if (((charData[col] >> row) & 0x01) == 1)
            {
                // Draw a rectangle with "pixelSize" side lengths to represent each pixel in the text
                tigrFill(_Screen, x + col * pixelSize, y + row * pixelSize, pixelSize, pixelSize, color);
            }
        }
    }
    dashMutex.unlock();
}