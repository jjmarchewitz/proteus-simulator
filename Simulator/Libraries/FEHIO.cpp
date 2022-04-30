#include "FEHIO.h"
#include "FEHDashboard.h"
#include <iostream>

bool FEHIO::InitializedPins[37] = {false};

/////////////////////
// DigitalInputPin
/////////////////////

DigitalInputPin::DigitalInputPin(FEHIO::FEHIOPin pin)
{
    Initialize(pin);
}

DigitalInputPin::DigitalInputPin()
{
    // This is that so students see "UNKWN" as the pin value and then use the other constructor
    Initialize(FEHIO::BATTERY_VOLTAGE);
}

void DigitalInputPin::Initialize(FEHIO::FEHIOPin pin)
{
    // The full pin ID would be the int value in the FEHIOPin enum representing something like "P2_5"
    _FullPinID = pin;
    // The bank would be the "2" in "P2_5"
    _Bank = pin / 8;
    // The pin num in the bank would be the "5" in "P2_5"
    _PinNumInBank = pin % 8;

    if (FEHIO::InitializedPins[_FullPinID] == true)
    {
        char errorString[80];
        sprintf(errorString, "\n\nThere is already a pin initialized at FEHIOPin::P%d_%d!\n", _Bank, _PinNumInBank);
        throw std::runtime_error(errorString);
    }
    else
    {
        FEHIO::InitializedPins[_FullPinID] = true;
    }

    _Value = false;
    _CurrentlyPressed = false;
    _PreviouslyPressed = false;

    // Get the full dimensions of the current cell. This cell acts as the entire space we have to draw the pin
    Dashboard.GetCellDimensions(&_Width, &_Height);

    _StartX = (3 + _PinNumInBank) * _Width;
    _StartY = _Bank * _Height;

    // Button drawing variables
    _ButtonStartX = _StartX + 5;
    _ButtonStartY = _StartY + 30;
    _ButtonWidth = _Width - 10;
    _ButtonHeight = _Height - 35;
}

bool DigitalInputPin::Value()
{

    // TODO: Document the bug related to clicking: on macOS

    int mouseButton, x_pos, y_pos;

    _PreviouslyPressed = _CurrentlyPressed;
    tigrMouse(Dashboard.Screen(), &x_pos, &y_pos, &mouseButton);

    // Update _currentlyPressed to reflect if the mouse is currently pressed or not
    if ((mouseButton & 0x01) == 1)
    {
        _CurrentlyPressed = true;
    }
    else
    {
        _CurrentlyPressed = false;
    }

    // If the mouse was just pressed (was not the last time this function was called),
    // and the mouse x position is inside the button, and the mouse y position is inside the button
    if (Dashboard.MouseWasJustClicked() && _CurrentlyPressed && !_PreviouslyPressed && x_pos >= _ButtonStartX && x_pos < (_ButtonStartX + _ButtonWidth) && y_pos >= _ButtonStartY && y_pos < (_ButtonStartY + _ButtonHeight))
    {
        // Use the XOR operator to flip the state of _value
        _Value = _Value ^ true;
    }

    return _Value;
}

void DigitalInputPin::Draw()
{
    TPixel color;
    const char *valueChar;

    // Value() is called instead of using _value because Value() will actually check and update the status of _value
    // If pin is true:
    if (Value() == true)
    {
        // Set color to green, character to T
        color = FEHDashboard::GreenColor;
        valueChar = "T";
    }
    else
    {
        // Set color to red, character to F
        color = FEHDashboard::ScarletColor;
        valueChar = "F";
    }

    dashMutex.lock();
    // Draw border rectangle
    tigrFill(Dashboard.Screen(), _ButtonStartX, _ButtonStartY, _ButtonWidth, _ButtonHeight, FEHDashboard::BlackColor);
    // Draw inner pin color/text
    tigrFill(Dashboard.Screen(), _ButtonStartX + _BorderSize, _ButtonStartY + _BorderSize, _ButtonWidth - 2 * _BorderSize, _ButtonHeight - 2 * _BorderSize, color);
    dashMutex.unlock();

    // No mutex lock because WriteAt has its own
    Dashboard.WriteAt("DIGITAL INPUT PIN", _StartX + 8, _StartY + 20, FEHDashboard::BlueColor, 1);
    // Write a character in the middle of the box representing true or false. This is for any red-green colorblind people
    Dashboard.WriteAt(valueChar, _StartX + (_Width / 2) - 14, _StartY + (_Height / 2) - 0, FEHDashboard::BlackColor, 4);
}

/////////////////////
// DigitalEncoder
/////////////////////

DigitalEncoder::DigitalEncoder(FEHIO::FEHIOPin pin, FEHIO::FEHIOInterruptTrigger trigger)
{
    Initialize(pin);
}

DigitalEncoder::DigitalEncoder(FEHIO::FEHIOPin pin)
{
    Initialize(pin);
}

int DigitalEncoder::Counts()
{
    return _Count;
}

void DigitalEncoder::ResetCounts()
{
    _Count = 0;
}

DigitalEncoder::DigitalEncoder()
{
    // This is that so students see "UNKWN" as the pin value and then use the other constructor
    Initialize(FEHIO::BATTERY_VOLTAGE);
}

void DigitalEncoder::Initialize(FEHIO::FEHIOPin pin)
{
    // The full pin ID would be the int value in the FEHIOPin enum representing something like "P2_5"
    _FullPinID = pin;
    // The bank would be the "2" in "P2_5"
    _Bank = pin / 8;
    // The pin num in the bank would be the "5" in "P2_5"
    _PinNumInBank = pin % 8;

    if (FEHIO::InitializedPins[_FullPinID] == true)
    {
        char errorString[80];
        sprintf(errorString, "\n\nThere is already a pin initialized at FEHIOPin::P%d_%d!\n", _Bank, _PinNumInBank);
        throw std::runtime_error(errorString);
    }
    else
    {
        FEHIO::InitializedPins[_FullPinID] = true;
    }

    // Reset the encoder to 0
    ResetCounts();

    // Get the full dimensions of the current cell. This cell acts as the entire space we have to draw the pin
    Dashboard.GetCellDimensions(&_Width, &_Height);

    _StartX = (3 + _PinNumInBank) * _Width;
    _StartY = _Bank * _Height;

    // Define button parameters

    // Plus 10
    _Buttons[_ButtonIDs::PlusTen].Text = "+10";
    _Buttons[_ButtonIDs::PlusTen].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::PlusTen].StartY = _StartY + 29;
    _Buttons[_ButtonIDs::PlusTen].Width = 49;
    _Buttons[_ButtonIDs::PlusTen].Height = 22;
    _Buttons[_ButtonIDs::PlusTen].TextOffsetX = 5;
    _Buttons[_ButtonIDs::PlusTen].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusTen].FontSize = 2;

    // Plus 100
    _Buttons[_ButtonIDs::PlusHundred].Text = "+100";
    _Buttons[_ButtonIDs::PlusHundred].StartX = _Buttons[_ButtonIDs::PlusTen].StartX + _Buttons[_ButtonIDs::PlusTen].Width + 2;
    _Buttons[_ButtonIDs::PlusHundred].StartY = _Buttons[_ButtonIDs::PlusTen].StartY;
    _Buttons[_ButtonIDs::PlusHundred].Width = 59;
    _Buttons[_ButtonIDs::PlusHundred].Height = 22;
    _Buttons[_ButtonIDs::PlusHundred].TextOffsetX = 4;
    _Buttons[_ButtonIDs::PlusHundred].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusHundred].FontSize = 2;

    // Plus 1000
    _Buttons[_ButtonIDs::PlusThousand].Text = "+1000";
    _Buttons[_ButtonIDs::PlusThousand].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::PlusThousand].StartY = _Buttons[_ButtonIDs::PlusTen].StartY + _Buttons[_ButtonIDs::PlusTen].Height + 2;
    _Buttons[_ButtonIDs::PlusThousand].Width = 110;
    _Buttons[_ButtonIDs::PlusThousand].Height = 22;
    _Buttons[_ButtonIDs::PlusThousand].TextOffsetX = 24;
    _Buttons[_ButtonIDs::PlusThousand].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusThousand].FontSize = 2;

    // Minus 10
    _Buttons[_ButtonIDs::MinusTen].Text = "-10";
    _Buttons[_ButtonIDs::MinusTen].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::MinusTen].StartY = _Buttons[_ButtonIDs::PlusThousand].StartY + 2 * _Buttons[_ButtonIDs::PlusThousand].Height + 4; // The extra numbers here are to account for the "Count: " display in the middle of the cell
    _Buttons[_ButtonIDs::MinusTen].Width = 49;
    _Buttons[_ButtonIDs::MinusTen].Height = 22;
    _Buttons[_ButtonIDs::MinusTen].TextOffsetX = 5;
    _Buttons[_ButtonIDs::MinusTen].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusTen].FontSize = 2;

    // Minus 100
    _Buttons[_ButtonIDs::MinusHundred].Text = "-100";
    _Buttons[_ButtonIDs::MinusHundred].StartX = _Buttons[_ButtonIDs::MinusTen].StartX + _Buttons[_ButtonIDs::MinusTen].Width + 2;
    _Buttons[_ButtonIDs::MinusHundred].StartY = _Buttons[_ButtonIDs::MinusTen].StartY;
    _Buttons[_ButtonIDs::MinusHundred].Width = 59;
    _Buttons[_ButtonIDs::MinusHundred].Height = 22;
    _Buttons[_ButtonIDs::MinusHundred].TextOffsetX = 4;
    _Buttons[_ButtonIDs::MinusHundred].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusHundred].FontSize = 2;

    // Minus 1000
    _Buttons[_ButtonIDs::MinusThousand].Text = "-1000";
    _Buttons[_ButtonIDs::MinusThousand].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::MinusThousand].StartY = _Buttons[_ButtonIDs::MinusHundred].StartY + _Buttons[_ButtonIDs::MinusHundred].Height + 2;
    _Buttons[_ButtonIDs::MinusThousand].Width = 110;
    _Buttons[_ButtonIDs::MinusThousand].Height = 22;
    _Buttons[_ButtonIDs::MinusThousand].TextOffsetX = 24;
    _Buttons[_ButtonIDs::MinusThousand].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusThousand].FontSize = 2;

    // Reset to 0
    _Buttons[_ButtonIDs::Reset].Text = "RESET";
    _Buttons[_ButtonIDs::Reset].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::Reset].StartY = _Buttons[_ButtonIDs::MinusThousand].StartY + _Buttons[_ButtonIDs::MinusThousand].Height + 2;
    _Buttons[_ButtonIDs::Reset].Width = 110;
    _Buttons[_ButtonIDs::Reset].Height = 22;
    _Buttons[_ButtonIDs::Reset].TextOffsetX = 24;
    _Buttons[_ButtonIDs::Reset].TextOffsetY = 4;
    _Buttons[_ButtonIDs::Reset].FontSize = 2;

    // Set the button pressed flags to both be false
    for (int i = 0; i < _NumOfButtons; i++)
    {
        _Buttons[i].CurrentlyPressed = false;
        _Buttons[i].PreviouslyPressed = false;
    }
}

void DigitalEncoder::Draw()
{
    TPixel color;
    int mouseButton, x_pos, y_pos;
    tigrMouse(Dashboard.Screen(), &x_pos, &y_pos, &mouseButton);

    for (int i = 0; i < _NumOfButtons; i++)
    {
        _Buttons[i].PreviouslyPressed = _Buttons[i].CurrentlyPressed;

        // Update CurrentlyPressed to reflect if the mouse is currently pressing the button at _Button[i]
        if ((mouseButton & 0x01) == 1 && x_pos > _Buttons[i].StartX && x_pos < _Buttons[i].StartX + _Buttons[i].Width && y_pos > _Buttons[i].StartY && y_pos < _Buttons[i].StartY + _Buttons[i].Height)
        {
            _Buttons[i].CurrentlyPressed = true;

            // I like the look of this color instead of the green color already added
            color = FEHDashboard::GreenColor;
        }
        else
        {
            _Buttons[i].CurrentlyPressed = false;
            color = FEHDashboard::ScarletColor;
        }

        dashMutex.lock();
        // Draw black background for border
        tigrFill(Dashboard.Screen(), _Buttons[i].StartX, _Buttons[i].StartY, _Buttons[i].Width, _Buttons[i].Height, FEHDashboard::BlackColor);
        // Draw button body
        tigrFill(Dashboard.Screen(), _Buttons[i].StartX + _BorderSize, _Buttons[i].StartY + _BorderSize, _Buttons[i].Width - 2 * _BorderSize, _Buttons[i].Height - 2 * _BorderSize, color);
        dashMutex.unlock();

        // Convert the std::string to a c string to match WriteAt's arguments.
        Dashboard.WriteAt(_Buttons[i].Text.c_str(), _Buttons[i].StartX + _Buttons[i].TextOffsetX, _Buttons[i].StartY + _Buttons[i].TextOffsetY, FEHDashboard::BlackColor, _Buttons[i].FontSize);
    }

    // No mutex lock because WriteAt has its own
    Dashboard.WriteAt("DIGITAL ENCODER", _StartX + 13, _StartY + 20, FEHDashboard::BlueColor, 1);

    // Draw the encoder count background and write the current count of the encoder to screen
    dashMutex.lock();
    tigrFill(Dashboard.Screen(), _StartX + 5, _Buttons[_ButtonIDs::PlusThousand].StartY + _Buttons[_ButtonIDs::PlusThousand].Height + 2, 110, 22, FEHDashboard::BlackColor);
    tigrFill(Dashboard.Screen(), _StartX + 5 + _BorderSize, _Buttons[_ButtonIDs::PlusThousand].StartY + _Buttons[_ButtonIDs::PlusThousand].Height + 2 + _BorderSize, 110 - 2 * _BorderSize, 22 - 2 * _BorderSize, FEHDashboard::BlueColor);
    dashMutex.unlock();
    sprintf(_CountString, "C:%d", Counts());
    Dashboard.WriteAt(_CountString, _StartX + 7, _Buttons[_ButtonIDs::PlusThousand].StartY + _Buttons[_ButtonIDs::PlusThousand].Height + 6, FEHDashboard::BlackColor, 2);

    // Update the count of the encoder based on which buttons were just pressed
    _UpdateCountFromPressedButtons();
}

void DigitalEncoder::_UpdateCountFromPressedButtons()
{
    // This prevents other buttons from being pressed if you hold down the mouse and hover over a bunch of buttons
    if (Dashboard.MouseWasJustClicked())
    {
        // Plus 10
        if (_Buttons[_ButtonIDs::PlusTen].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusTen].PreviouslyPressed)
        {
            _Count += 10;
        }
        // Plus 100
        else if (_Buttons[_ButtonIDs::PlusHundred].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusHundred].PreviouslyPressed)
        {
            _Count += 100;
        }
        // Plus 1000
        else if (_Buttons[_ButtonIDs::PlusThousand].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusThousand].PreviouslyPressed)
        {
            _Count += 1000;
        }
        // Minus 10
        else if (_Buttons[_ButtonIDs::MinusTen].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusTen].PreviouslyPressed)
        {
            _Count -= 10;
        }
        // Minus 100
        else if (_Buttons[_ButtonIDs::MinusHundred].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusHundred].PreviouslyPressed)
        {
            _Count -= 100;
        }
        // Minus 1000
        else if (_Buttons[_ButtonIDs::MinusThousand].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusThousand].PreviouslyPressed)
        {
            _Count -= 1000;
        }
        // Reset
        else if (_Buttons[_ButtonIDs::Reset].CurrentlyPressed && !_Buttons[_ButtonIDs::Reset].PreviouslyPressed)
        {
            ResetCounts();
        }
    }

    if (_Count < 0)
    {
        ResetCounts();
    }

    // If you're reading this, listen to the live concert recording on YouTube called "Vulfpeck /// Live at Madison Square Garden".
    // It's great working music, helping me make a big push on this simulator! :D
}

/////////////////////
// DigitalOutputPin
/////////////////////

DigitalOutputPin::DigitalOutputPin(FEHIO::FEHIOPin pin)
{
    // The full pin ID would be the int value in the FEHIOPin enum representing something like "P2_5"
    _FullPinID = pin;
    // The bank would be the "2" in "P2_5"
    _Bank = pin / 8;
    // The pin num in the bank would be the "5" in "P2_5"
    _PinNumInBank = pin % 8;

    if (FEHIO::InitializedPins[_FullPinID] == true)
    {
        char errorString[80];
        sprintf(errorString, "\n\nThere is already a pin initialized at FEHIOPin::P%d_%d!\n", _Bank, _PinNumInBank);
        throw std::runtime_error(errorString);
    }
    else
    {
        FEHIO::InitializedPins[_FullPinID] = true;
    }

    _Value = false;

    // Get the full dimensions of the current cell. This cell acts as the entire space we have to draw the pin
    Dashboard.GetCellDimensions(&_Width, &_Height);

    _StartX = (3 + _PinNumInBank) * _Width;
    _StartY = _Bank * _Height;

    // Box drawing variables
    _BoxStartX = _StartX + 5;
    _BoxStartY = _StartY + 30;
    _BoxWidth = _Width - 10;
    _BoxHeight = _Height - 35;
}

void DigitalOutputPin::Write(bool newValue)
{
    _Value = newValue;
}

bool DigitalOutputPin::Status()
{
    return _Value;
}

void DigitalOutputPin::Toggle()
{
    // XOR operator acts like a toggle when one of the inputs is a 1
    _Value = _Value ^ true;
}

void DigitalOutputPin::Draw()
{
    TPixel color;
    const char *valueChar;

    // If pin is true:
    if (_Value == true)
    {
        // Set color to green, character to T
        color = FEHDashboard::GreenColor;
        valueChar = "T";
    }
    else
    {
        // Set color to red, character to F
        color = FEHDashboard::ScarletColor;
        valueChar = "F";
    }

    dashMutex.lock();
    // Draw border rectangle
    tigrFill(Dashboard.Screen(), _BoxStartX, _BoxStartY, _BoxWidth, _BoxHeight, FEHDashboard::BlackColor);
    // Draw inner pin color/text
    tigrFill(Dashboard.Screen(), _BoxStartX + _BorderSize, _BoxStartY + _BorderSize, _BoxWidth - 2 * _BorderSize, _BoxHeight - 2 * _BorderSize, color);
    dashMutex.unlock();

    // No mutex lock because WriteAt has its own
    Dashboard.WriteAt("DIGITAL OUTPUT PIN", _StartX + 5, _StartY + 20, FEHDashboard::BlueColor, 1);
    // Write a character in the middle of the box representing true or false. This is for any red-green colorblind people
    Dashboard.WriteAt(valueChar, _StartX + (_Width / 2) - 14, _StartY + (_Height / 2) - 0, FEHDashboard::BlackColor, 4);
}

/////////////////////
// AnalogInputPin
/////////////////////

AnalogInputPin::AnalogInputPin(FEHIO::FEHIOPin pin)
{
    // The full pin ID would be the int value in the FEHIOPin enum representing something like "P2_5"
    _FullPinID = pin;
    // The bank would be the "2" in "P2_5"
    _Bank = pin / 8;
    // The pin num in the bank would be the "5" in "P2_5"
    _PinNumInBank = pin % 8;

    if (FEHIO::InitializedPins[_FullPinID] == true)
    {
        char errorString[80];
        sprintf(errorString, "\n\nThere is already a pin initialized at FEHIOPin::P%d_%d!\n", _Bank, _PinNumInBank);
        throw std::runtime_error(errorString);
    }
    else
    {
        FEHIO::InitializedPins[_FullPinID] = true;
    }

    // Reset the value of the pin to 0
    _Value = 0;

    // Get the full dimensions of the current cell. This cell acts as the entire space we have to draw the pin
    Dashboard.GetCellDimensions(&_Width, &_Height);

    _StartX = (3 + _PinNumInBank) * _Width;
    _StartY = _Bank * _Height;

    // Define button parameters

    // Plus 0.01
    _Buttons[_ButtonIDs::PlusHundredth].Text = "+0.01";
    _Buttons[_ButtonIDs::PlusHundredth].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::PlusHundredth].StartY = _StartY + 29;
    _Buttons[_ButtonIDs::PlusHundredth].Width = 110;
    _Buttons[_ButtonIDs::PlusHundredth].Height = 22;
    _Buttons[_ButtonIDs::PlusHundredth].TextOffsetX = 25;
    _Buttons[_ButtonIDs::PlusHundredth].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusHundredth].FontSize = 2;

    // Plus 0.1
    _Buttons[_ButtonIDs::PlusTenth].Text = "+0.1";
    _Buttons[_ButtonIDs::PlusTenth].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::PlusTenth].StartY = _Buttons[_ButtonIDs::PlusHundredth].StartY + _Buttons[_ButtonIDs::PlusHundredth].Height + 2;
    _Buttons[_ButtonIDs::PlusTenth].Width = 59;
    _Buttons[_ButtonIDs::PlusTenth].Height = 22;
    _Buttons[_ButtonIDs::PlusTenth].TextOffsetX = 5;
    _Buttons[_ButtonIDs::PlusTenth].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusTenth].FontSize = 2;

    // Plus 1
    _Buttons[_ButtonIDs::PlusOne].Text = "+1";
    _Buttons[_ButtonIDs::PlusOne].StartX = _Buttons[_ButtonIDs::PlusTenth].StartX + _Buttons[_ButtonIDs::PlusTenth].Width + 2;
    _Buttons[_ButtonIDs::PlusOne].StartY = _Buttons[_ButtonIDs::PlusTenth].StartY;
    _Buttons[_ButtonIDs::PlusOne].Width = 49;
    _Buttons[_ButtonIDs::PlusOne].Height = 22;
    _Buttons[_ButtonIDs::PlusOne].TextOffsetX = 12;
    _Buttons[_ButtonIDs::PlusOne].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusOne].FontSize = 2;

    // Minus 0.01
    _Buttons[_ButtonIDs::MinusHundredth].Text = "-0.01";
    _Buttons[_ButtonIDs::MinusHundredth].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::MinusHundredth].StartY = _Buttons[_ButtonIDs::PlusOne].StartY + 2 * _Buttons[_ButtonIDs::PlusOne].Height + 4; // The extra numbers here are to account for the "Count: " display in the middle of the cell;
    _Buttons[_ButtonIDs::MinusHundredth].Width = 110;
    _Buttons[_ButtonIDs::MinusHundredth].Height = 22;
    _Buttons[_ButtonIDs::MinusHundredth].TextOffsetX = 25;
    _Buttons[_ButtonIDs::MinusHundredth].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusHundredth].FontSize = 2;

    // Minus 0.1
    _Buttons[_ButtonIDs::MinusTenth].Text = "-0.1";
    _Buttons[_ButtonIDs::MinusTenth].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::MinusTenth].StartY = _Buttons[_ButtonIDs::MinusHundredth].StartY + _Buttons[_ButtonIDs::MinusHundredth].Height + 2;
    _Buttons[_ButtonIDs::MinusTenth].Width = 59;
    _Buttons[_ButtonIDs::MinusTenth].Height = 22;
    _Buttons[_ButtonIDs::MinusTenth].TextOffsetX = 5;
    _Buttons[_ButtonIDs::MinusTenth].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusTenth].FontSize = 2;

    // Minus 1
    _Buttons[_ButtonIDs::MinusOne].Text = "-1";
    _Buttons[_ButtonIDs::MinusOne].StartX = _Buttons[_ButtonIDs::MinusTenth].StartX + _Buttons[_ButtonIDs::MinusTenth].Width + 2;
    _Buttons[_ButtonIDs::MinusOne].StartY = _Buttons[_ButtonIDs::MinusTenth].StartY;
    _Buttons[_ButtonIDs::MinusOne].Width = 49;
    _Buttons[_ButtonIDs::MinusOne].Height = 22;
    _Buttons[_ButtonIDs::MinusOne].TextOffsetX = 12;
    _Buttons[_ButtonIDs::MinusOne].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusOne].FontSize = 2;

    // Reset to 0
    _Buttons[_ButtonIDs::Reset].Text = "RESET";
    _Buttons[_ButtonIDs::Reset].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::Reset].StartY = _Buttons[_ButtonIDs::MinusOne].StartY + _Buttons[_ButtonIDs::MinusOne].Height + 2;
    _Buttons[_ButtonIDs::Reset].Width = 110;
    _Buttons[_ButtonIDs::Reset].Height = 22;
    _Buttons[_ButtonIDs::Reset].TextOffsetX = 24;
    _Buttons[_ButtonIDs::Reset].TextOffsetY = 4;
    _Buttons[_ButtonIDs::Reset].FontSize = 2;

    // Set the button pressed flags to both be false
    for (int i = 0; i < _NumOfButtons; i++)
    {
        _Buttons[i].CurrentlyPressed = false;
        _Buttons[i].PreviouslyPressed = false;
    }
}

float AnalogInputPin::Value()
{
    return _Value;
}

void AnalogInputPin::Draw()
{
    // The battery uses a special AnalogInputPin with an ID of FEHIO::BATTERY_VOLTAGE, and this prevents
    // an extra pin being drawn. For some reason only on Windows it was showing up at P0_0
    if (_FullPinID != FEHIO::BATTERY_VOLTAGE)
    {
        TPixel color;
        int mouseButton, x_pos, y_pos;
        tigrMouse(Dashboard.Screen(), &x_pos, &y_pos, &mouseButton);

        for (int i = 0; i < _NumOfButtons; i++)
        {
            _Buttons[i].PreviouslyPressed = _Buttons[i].CurrentlyPressed;

            // Update CurrentlyPressed to reflect if the mouse is currently pressing the button at _Button[i]
            if ((mouseButton & 0x01) == 1 && x_pos > _Buttons[i].StartX && x_pos < _Buttons[i].StartX + _Buttons[i].Width && y_pos > _Buttons[i].StartY && y_pos < _Buttons[i].StartY + _Buttons[i].Height)
            {
                _Buttons[i].CurrentlyPressed = true;
                color = FEHDashboard::GreenColor;
            }
            else
            {
                _Buttons[i].CurrentlyPressed = false;
                color = FEHDashboard::ScarletColor;
            }

            dashMutex.lock();
            // Draw black background for border
            tigrFill(Dashboard.Screen(), _Buttons[i].StartX, _Buttons[i].StartY, _Buttons[i].Width, _Buttons[i].Height, FEHDashboard::BlackColor);
            // Draw button body
            tigrFill(Dashboard.Screen(), _Buttons[i].StartX + _BorderSize, _Buttons[i].StartY + _BorderSize, _Buttons[i].Width - 2 * _BorderSize, _Buttons[i].Height - 2 * _BorderSize, color);
            dashMutex.unlock();

            // Convert the std::string to a c string to match WriteAt's arguments.
            Dashboard.WriteAt(_Buttons[i].Text.c_str(), _Buttons[i].StartX + _Buttons[i].TextOffsetX, _Buttons[i].StartY + _Buttons[i].TextOffsetY, FEHDashboard::BlackColor, _Buttons[i].FontSize);
        }

        // No mutex lock because WriteAt has its own
        Dashboard.WriteAt("ANALOG INPUT PIN", _StartX + 11, _StartY + 20, FEHDashboard::BlueColor, 1);

        // Draw the encoder count background and write the current voltage of the pin to screen
        dashMutex.lock();
        tigrFill(Dashboard.Screen(), _StartX + 5, _Buttons[_ButtonIDs::PlusTenth].StartY + _Buttons[_ButtonIDs::PlusTenth].Height + 2, 110, 22, FEHDashboard::BlackColor);
        tigrFill(Dashboard.Screen(), _StartX + 5 + _BorderSize, _Buttons[_ButtonIDs::PlusTenth].StartY + _Buttons[_ButtonIDs::PlusTenth].Height + 2 + _BorderSize, 110 - 2 * _BorderSize, 22 - 2 * _BorderSize, FEHDashboard::BlueColor);
        dashMutex.unlock();
        sprintf(_ValueString, "%2.3f V", Value());
        Dashboard.WriteAt(_ValueString, _StartX + 7, _Buttons[_ButtonIDs::PlusTenth].StartY + _Buttons[_ButtonIDs::PlusTenth].Height + 6, FEHDashboard::BlackColor, 2);

        // Update the voltage of the pin based on which buttons were just pressed
        _UpdateValueFromPressedButtons();
    }
}

void AnalogInputPin::_UpdateValueFromPressedButtons()
{
    // This prevents other buttons from being pressed if you hold down the mouse and hover over a bunch of buttons
    if (Dashboard.MouseWasJustClicked())
    {
        // Plus 0.01
        if (_Buttons[_ButtonIDs::PlusHundredth].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusHundredth].PreviouslyPressed)
        {
            _Value += 0.01;
        }
        // Plus 0.1
        else if (_Buttons[_ButtonIDs::PlusTenth].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusTenth].PreviouslyPressed)
        {
            _Value += 0.1;
        }
        // Plus 1
        else if (_Buttons[_ButtonIDs::PlusOne].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusOne].PreviouslyPressed)
        {
            _Value += 1;
        }
        // Minus 0.01
        else if (_Buttons[_ButtonIDs::MinusHundredth].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusHundredth].PreviouslyPressed)
        {
            _Value -= 0.01;
        }
        // Minus 0.1
        else if (_Buttons[_ButtonIDs::MinusTenth].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusTenth].PreviouslyPressed)
        {
            _Value -= 0.1;
        }
        // Minus 1
        else if (_Buttons[_ButtonIDs::MinusOne].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusOne].PreviouslyPressed)
        {
            _Value -= 1;
        }
        // Reset
        else if (_Buttons[_ButtonIDs::Reset].CurrentlyPressed && !_Buttons[_ButtonIDs::Reset].PreviouslyPressed)
        {
            _Value = 0;
        }
    }

    // Make sure the value can't be negative
    if (_Value < 0)
    {
        _Value = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// DEPRECATED: ButtonBoard
// These functions do not need to be implemented because the button board has been deprecated
////////////////////////////////////////////////////////////////////////////////////////////////////

ButtonBoard::ButtonBoard(FEHIO::FEHIOPort bank)
{
    std::cout << "The button board has been deprecated and is not implemented on the simulator. ";
    std::cout << "If you believe that this should not be the case, please let a TA know so that ";
    std::cout << "they can get in contact with the people currently working on the simulator.";
}
bool ButtonBoard::LeftPressed() { return false; }
bool ButtonBoard::LeftReleased() { return false; }
bool ButtonBoard::MiddlePressed() { return false; }
bool ButtonBoard::MiddleReleased() { return false; }
bool ButtonBoard::RightPressed() { return false; }
bool ButtonBoard::RightReleased() { return false; }

/////////////////////
// AnalogEncoder
/////////////////////

void AnalogEncoder::SetRate(unsigned int rateHz) {}

AnalogEncoder::AnalogEncoder(FEHIO::FEHIOPin pin)
{
    // The full pin ID would be the int value in the FEHIOPin enum representing something like "P2_5"
    _FullPinID = pin;
    // The bank would be the "2" in "P2_5"
    _Bank = pin / 8;
    // The pin num in the bank would be the "5" in "P2_5"
    _PinNumInBank = pin % 8;

    if (FEHIO::InitializedPins[_FullPinID] == true)
    {
        char errorString[80];
        sprintf(errorString, "\n\nThere is already a pin initialized at FEHIOPin::P%d_%d!\n", _Bank, _PinNumInBank);
        throw std::runtime_error(errorString);
    }
    else
    {
        FEHIO::InitializedPins[_FullPinID] = true;
    }

    // Reset the encoder to 0
    ResetCounts();

    // Get the full dimensions of the current cell. This cell acts as the entire space we have to draw the pin
    Dashboard.GetCellDimensions(&_Width, &_Height);

    _StartX = (3 + _PinNumInBank) * _Width;
    _StartY = _Bank * _Height;

    // Define button parameters

    // Plus 10
    _Buttons[_ButtonIDs::PlusTen].Text = "+10";
    _Buttons[_ButtonIDs::PlusTen].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::PlusTen].StartY = _StartY + 29;
    _Buttons[_ButtonIDs::PlusTen].Width = 49;
    _Buttons[_ButtonIDs::PlusTen].Height = 22;
    _Buttons[_ButtonIDs::PlusTen].TextOffsetX = 5;
    _Buttons[_ButtonIDs::PlusTen].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusTen].FontSize = 2;

    // Plus 100
    _Buttons[_ButtonIDs::PlusHundred].Text = "+100";
    _Buttons[_ButtonIDs::PlusHundred].StartX = _Buttons[_ButtonIDs::PlusTen].StartX + _Buttons[_ButtonIDs::PlusTen].Width + 2;
    _Buttons[_ButtonIDs::PlusHundred].StartY = _Buttons[_ButtonIDs::PlusTen].StartY;
    _Buttons[_ButtonIDs::PlusHundred].Width = 59;
    _Buttons[_ButtonIDs::PlusHundred].Height = 22;
    _Buttons[_ButtonIDs::PlusHundred].TextOffsetX = 4;
    _Buttons[_ButtonIDs::PlusHundred].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusHundred].FontSize = 2;

    // Plus 1000
    _Buttons[_ButtonIDs::PlusThousand].Text = "+1000";
    _Buttons[_ButtonIDs::PlusThousand].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::PlusThousand].StartY = _Buttons[_ButtonIDs::PlusTen].StartY + _Buttons[_ButtonIDs::PlusTen].Height + 2;
    _Buttons[_ButtonIDs::PlusThousand].Width = 110;
    _Buttons[_ButtonIDs::PlusThousand].Height = 22;
    _Buttons[_ButtonIDs::PlusThousand].TextOffsetX = 24;
    _Buttons[_ButtonIDs::PlusThousand].TextOffsetY = 4;
    _Buttons[_ButtonIDs::PlusThousand].FontSize = 2;

    // Minus 10
    _Buttons[_ButtonIDs::MinusTen].Text = "-10";
    _Buttons[_ButtonIDs::MinusTen].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::MinusTen].StartY = _Buttons[_ButtonIDs::PlusThousand].StartY + 2 * _Buttons[_ButtonIDs::PlusThousand].Height + 4; // The extra numbers here are to account for the "Count: " display in the middle of the cell
    _Buttons[_ButtonIDs::MinusTen].Width = 49;
    _Buttons[_ButtonIDs::MinusTen].Height = 22;
    _Buttons[_ButtonIDs::MinusTen].TextOffsetX = 5;
    _Buttons[_ButtonIDs::MinusTen].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusTen].FontSize = 2;

    // Minus 100
    _Buttons[_ButtonIDs::MinusHundred].Text = "-100";
    _Buttons[_ButtonIDs::MinusHundred].StartX = _Buttons[_ButtonIDs::MinusTen].StartX + _Buttons[_ButtonIDs::MinusTen].Width + 2;
    _Buttons[_ButtonIDs::MinusHundred].StartY = _Buttons[_ButtonIDs::MinusTen].StartY;
    _Buttons[_ButtonIDs::MinusHundred].Width = 59;
    _Buttons[_ButtonIDs::MinusHundred].Height = 22;
    _Buttons[_ButtonIDs::MinusHundred].TextOffsetX = 4;
    _Buttons[_ButtonIDs::MinusHundred].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusHundred].FontSize = 2;

    // Minus 1000
    _Buttons[_ButtonIDs::MinusThousand].Text = "-1000";
    _Buttons[_ButtonIDs::MinusThousand].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::MinusThousand].StartY = _Buttons[_ButtonIDs::MinusHundred].StartY + _Buttons[_ButtonIDs::MinusHundred].Height + 2;
    _Buttons[_ButtonIDs::MinusThousand].Width = 110;
    _Buttons[_ButtonIDs::MinusThousand].Height = 22;
    _Buttons[_ButtonIDs::MinusThousand].TextOffsetX = 24;
    _Buttons[_ButtonIDs::MinusThousand].TextOffsetY = 4;
    _Buttons[_ButtonIDs::MinusThousand].FontSize = 2;

    // Reset to 0
    _Buttons[_ButtonIDs::Reset].Text = "RESET";
    _Buttons[_ButtonIDs::Reset].StartX = _StartX + 5;
    _Buttons[_ButtonIDs::Reset].StartY = _Buttons[_ButtonIDs::MinusThousand].StartY + _Buttons[_ButtonIDs::MinusThousand].Height + 2;
    _Buttons[_ButtonIDs::Reset].Width = 110;
    _Buttons[_ButtonIDs::Reset].Height = 22;
    _Buttons[_ButtonIDs::Reset].TextOffsetX = 24;
    _Buttons[_ButtonIDs::Reset].TextOffsetY = 4;
    _Buttons[_ButtonIDs::Reset].FontSize = 2;

    // Set the button pressed flags to both be false
    for (int i = 0; i < _NumOfButtons; i++)
    {
        _Buttons[i].CurrentlyPressed = false;
        _Buttons[i].PreviouslyPressed = false;
    }
}

AnalogEncoder::~AnalogEncoder() {}

int AnalogEncoder::Counts()
{
    return _Count;
}

void AnalogEncoder::ResetCounts()
{
    _Count = 0;
}

void AnalogEncoder::SetThresholds(float low, float high)
{
    _LowThreshold = low;
    _HighThreshold = high;
}

void AnalogEncoder::Draw()
{
    TPixel color;
    int mouseButton, x_pos, y_pos;
    tigrMouse(Dashboard.Screen(), &x_pos, &y_pos, &mouseButton);

    for (int i = 0; i < _NumOfButtons; i++)
    {
        _Buttons[i].PreviouslyPressed = _Buttons[i].CurrentlyPressed;

        // Update CurrentlyPressed to reflect if the mouse is currently pressing the button at _Button[i]
        if ((mouseButton & 0x01) == 1 && x_pos > _Buttons[i].StartX && x_pos < _Buttons[i].StartX + _Buttons[i].Width && y_pos > _Buttons[i].StartY && y_pos < _Buttons[i].StartY + _Buttons[i].Height)
        {
            _Buttons[i].CurrentlyPressed = true;

            // I like the look of this color instead of the green color already added
            color = FEHDashboard::GreenColor;
        }
        else
        {
            _Buttons[i].CurrentlyPressed = false;
            color = FEHDashboard::ScarletColor;
        }

        dashMutex.lock();
        // Draw black background for border
        tigrFill(Dashboard.Screen(), _Buttons[i].StartX, _Buttons[i].StartY, _Buttons[i].Width, _Buttons[i].Height, FEHDashboard::BlackColor);
        // Draw button body
        tigrFill(Dashboard.Screen(), _Buttons[i].StartX + _BorderSize, _Buttons[i].StartY + _BorderSize, _Buttons[i].Width - 2 * _BorderSize, _Buttons[i].Height - 2 * _BorderSize, color);
        dashMutex.unlock();

        // Convert the std::string to a c string to match WriteAt's arguments.
        Dashboard.WriteAt(_Buttons[i].Text.c_str(), _Buttons[i].StartX + _Buttons[i].TextOffsetX, _Buttons[i].StartY + _Buttons[i].TextOffsetY, FEHDashboard::BlackColor, _Buttons[i].FontSize);
    }

    // No mutex lock because WriteAt has its own
    Dashboard.WriteAt("ANALOG ENCODER", _StartX + 16, _StartY + 20, FEHDashboard::BlueColor, 1);

    // Draw the encoder count background and write the current count of the encoder to screen
    dashMutex.lock();
    tigrFill(Dashboard.Screen(), _StartX + 5, _Buttons[_ButtonIDs::PlusThousand].StartY + _Buttons[_ButtonIDs::PlusThousand].Height + 2, 110, 22, FEHDashboard::BlackColor);
    tigrFill(Dashboard.Screen(), _StartX + 5 + _BorderSize, _Buttons[_ButtonIDs::PlusThousand].StartY + _Buttons[_ButtonIDs::PlusThousand].Height + 2 + _BorderSize, 110 - 2 * _BorderSize, 22 - 2 * _BorderSize, FEHDashboard::BlueColor);
    dashMutex.unlock();
    sprintf(_CountString, "C:%d", Counts());
    Dashboard.WriteAt(_CountString, _StartX + 7, _Buttons[_ButtonIDs::PlusThousand].StartY + _Buttons[_ButtonIDs::PlusThousand].Height + 6, FEHDashboard::BlackColor, 2);

    // Update the count of the encoder based on which buttons were just pressed
    _UpdateCountFromPressedButtons();
}

void AnalogEncoder::_UpdateCountFromPressedButtons()
{
    // This prevents other buttons from being pressed if you hold down the mouse and hover over a bunch of buttons
    if (Dashboard.MouseWasJustClicked())
    {
        // Plus 10
        if (_Buttons[_ButtonIDs::PlusTen].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusTen].PreviouslyPressed)
        {
            _Count += 10;
        }
        // Plus 100
        else if (_Buttons[_ButtonIDs::PlusHundred].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusHundred].PreviouslyPressed)
        {
            _Count += 100;
        }
        // Plus 1000
        else if (_Buttons[_ButtonIDs::PlusThousand].CurrentlyPressed && !_Buttons[_ButtonIDs::PlusThousand].PreviouslyPressed)
        {
            _Count += 1000;
        }
        // Minus 10
        else if (_Buttons[_ButtonIDs::MinusTen].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusTen].PreviouslyPressed)
        {
            _Count -= 10;
        }
        // Minus 100
        else if (_Buttons[_ButtonIDs::MinusHundred].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusHundred].PreviouslyPressed)
        {
            _Count -= 100;
        }
        // Minus 1000
        else if (_Buttons[_ButtonIDs::MinusThousand].CurrentlyPressed && !_Buttons[_ButtonIDs::MinusThousand].PreviouslyPressed)
        {
            _Count -= 1000;
        }
        // Reset
        else if (_Buttons[_ButtonIDs::Reset].CurrentlyPressed && !_Buttons[_ButtonIDs::Reset].PreviouslyPressed)
        {
            ResetCounts();
        }
    }

    // Make sure the count can't be negative
    if (_Count < 0)
    {
        ResetCounts();
    }
}
