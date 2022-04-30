#include "FEHServo.h"

bool FEHServo::InitializedServos[8] = {false}; // Even though there is only one element, this initializes the whole array to false

FEHServo::FEHServo(FEHServoPort port)
{
    _ServoPort = port;
    _position = -1;

    if (InitializedServos[_ServoPort] == true)
    {
        char errorString[80];
        sprintf(errorString, "\n\nThere is already a Servo initialized at FEHServoPort::Servo%d!\n", _ServoPort);
        throw std::runtime_error(errorString);
    }
    else
    {
        InitializedServos[_ServoPort] = true;
    }

    Dashboard.GetCellDimensions(&_Width, &_Height);

    if (_ServoPort < 4)
    {
        _StartX = _Width;
    }
    else
    {
        _StartX = 2 * _Width;
    }

    _StartY = (_ServoPort % 4) * _Height;

    // Bar drawing variables
    _BarWidth = 30;
    _BarStartX = _Width - _BarWidth - 5;
    _BarStartY = 30;
    _BarMaxHeight = _Height - _BarStartY - 5;

    // Text box variables
    _TextBoxStartX = 8;
    _TextBoxStartY = _BarStartY + (_BarMaxHeight / 2) - 11;
    _TextBoxWidth = 68;
    _TextBoxHeight = 22;
    _TextBoxTextOffsetX = 2;
    _TextBoxTextOffsetY = 4;
}

void FEHServo::SetDegree(float degree)
{
    if (degree < 0)
        _position = 0;
    else if (degree > 180)
        _position = 180;
    else
        _position = degree;
}

void FEHServo::TouchCalibrate() {}

void FEHServo::Off()
{
    _position = -1;
}

// SetMax and SetMin don't have any purpose in the simulator
void FEHServo::SetMax(int max) {}

void FEHServo::SetMin(int min) {}

// Debug functions:
void Calibrate() {} // Deprecated because uses button board
void FEHServo::DigitalOn() {}
void FEHServo::DigitalOff() {}

void FEHServo::Draw()
{
    dashMutex.lock();

    // Draw outline bar
    tigrFill(Dashboard.Screen(), _StartX + _BarStartX, _StartY + _BarStartY, _BarWidth, _BarMaxHeight, FEHDashboard::BlackColor);
    tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + _BorderSize, _BarWidth - 2 * _BorderSize, _BarMaxHeight - 2 * _BorderSize, FEHDashboard::BackgroundColor);

    // Draw text box outline
    tigrFill(Dashboard.Screen(), _StartX + _TextBoxStartX, _StartY + _TextBoxStartY, _TextBoxWidth, _TextBoxHeight, FEHDashboard::BlackColor);
    tigrFill(Dashboard.Screen(), _StartX + _BorderSize + _TextBoxStartX, _StartY + _BorderSize + _TextBoxStartY, _TextBoxWidth - 2 * _BorderSize, _TextBoxHeight - 2 * _BorderSize, FEHDashboard::BlueColor);

    // Draw percent bar
    if (_position >= 0 && _position <= 180)
    {
        float yPosition = ((180 - _position) / 180.0) * (_BarMaxHeight - _BorderSize);
        // Green filler bar
        tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + _BorderSize + (int)yPosition, _BarWidth - 2 * _BorderSize, _BarMaxHeight - (int)yPosition - 2 * _BorderSize, FEHDashboard::GreenColor);
        // Black percentage bar
        tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + (int)yPosition, _BarWidth - 2 * _BorderSize, _BorderSize, FEHDashboard::BlackColor);
        dashMutex.unlock();

        // Print the percent data to the screen
        char positionString[6]; // Maximum of 4 digits + '.' + \0
        sprintf(positionString, "%.1f", _position);
        Dashboard.WriteAt(positionString, _StartX + _TextBoxStartX + _TextBoxTextOffsetX, _StartY + _TextBoxStartY + _TextBoxTextOffsetY, Dashboard.BlackColor, 2);
    }
    // Servo is off
    else if (_position == -1)
    {
        dashMutex.unlock();

        Dashboard.WriteAt("Off", _StartX + _TextBoxStartX + _TextBoxTextOffsetX, _StartY + _TextBoxStartY + _TextBoxTextOffsetY, Dashboard.BlackColor, 2);
    }
}