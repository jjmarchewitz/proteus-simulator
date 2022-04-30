#include "FEHMotor.h"
#include <stdexcept>
#include <iostream>

bool FEHMotor::InitializedMotors[4] = {false}; // Even though there is only one element, this initializes the whole array to false

FEHMotor::FEHMotor(FEHMotorPort port, float voltage)
{
	_Percent = 0;
	_Port = port;
	// voltage is unused in the simulator

	if (InitializedMotors[_Port] == true)
	{
		char errorString[80];
		sprintf(errorString, "\n\nThere is already a motor initialized at FEHMotorPort::Motor%d!\n", _Port);
		throw std::runtime_error(errorString);
	}
	else
	{
		InitializedMotors[_Port] = true;
	}

	Dashboard.GetCellDimensions(&_Width, &_Height);

	_StartX = 0;
	_StartY = _Port * _Height;

	// Bar drawing variables
	_BarWidth = 30;
	_BarStartX = _Width - _BarWidth - 5;
	_BarStartY = 30;
	_BarMaxHeight = _Height - _BarStartY - 5;

	// Text bot variables
	_TextBoxStartX = 2;
	_TextBoxStartY = _BarStartY + (_BarMaxHeight / 2) - 11;
	_TextBoxWidth = 78;
	_TextBoxHeight = 22;
	_TextBoxTextOffsetX = 2;
	_TextBoxTextOffsetY = 4;
}

void FEHMotor::SetPercent(float percent)
{
	if (percent > 100)
	{
		percent = 100;
	}
	else if (percent < -100)
	{
		percent = -100;
	}

	dashMutex.lock();
	_Percent = percent;
	dashMutex.unlock();
}

void FEHMotor::Stop()
{
	SetPercent(0);
}

void FEHMotor::Draw()
{
	dashMutex.lock();

	// Creates a color based off of the current percent.  The higher the percent, the more green.  The lower, the more red.
	_Color = tigrRGB((char)(180.0 - (_Percent * 64.0 / 100.0) - 64.0), (char)((_Percent * 64.0 / 100.0) + 64.0), 0);

	// Draw percentage bar outline
	tigrFill(Dashboard.Screen(), _StartX + _BarStartX, _StartY + _BarStartY, _BarWidth, _BarMaxHeight, FEHDashboard::BlackColor);
	tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + _BorderSize, _BarWidth - 2 * _BorderSize, _BarMaxHeight - 2 * _BorderSize, FEHDashboard::BackgroundColor);

	// Black bar at zero percent (middle of the bar)
	tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + (_BarMaxHeight / 2) - (_BorderSize / 2), _BarWidth - 2 * _BorderSize, _BorderSize, FEHDashboard::BlackColor);

	// Draw text box outline
	tigrFill(Dashboard.Screen(), _StartX + _TextBoxStartX, _StartY + _TextBoxStartY, _TextBoxWidth, _TextBoxHeight, FEHDashboard::BlackColor);
	tigrFill(Dashboard.Screen(), _StartX + _TextBoxBorderSize + _TextBoxStartX, _StartY + _TextBoxBorderSize + _TextBoxStartY, _TextBoxWidth - 2 * _TextBoxBorderSize, _TextBoxHeight - 2 * _TextBoxBorderSize, FEHDashboard::BlueColor);

	// Not sure why 3 * _BorderSize is the magic number but it works better than 2 or 4
	int height = (float)((_BarMaxHeight - 3 * _BorderSize) / 2) * _Percent / 100.0;

	// Draw the colored-in motor percentage bar
	if (height < 0)
	{
		// Colored-in bar
		tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + (_BarMaxHeight / 2) + (_BorderSize / 2), _BarWidth - (_BorderSize * 2), -height, _Color);
		// Black bar to cap off the end of the colored-in bar
		tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + (_BarMaxHeight / 2) + (_BorderSize / 2) - height, _BarWidth - (_BorderSize * 2), _BorderSize, FEHDashboard::BlackColor);
	}
	else if (height > 0)
	{
		// Colored-in bar
		tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + ((_BarMaxHeight / 2) - height) - (_BorderSize / 2), _BarWidth - (_BorderSize * 2), height, _Color);
		// Black bar to cap off the end of the colored-in bar
		tigrFill(Dashboard.Screen(), _StartX + _BarStartX + _BorderSize, _StartY + _BarStartY + ((_BarMaxHeight / 2) - height) - 3 * (_BorderSize / 2), _BarWidth - (_BorderSize * 2), _BorderSize, FEHDashboard::BlackColor);
	}
	dashMutex.unlock();

	// Print the percent data to the screen
	char percentString[7]; // Maximum of 4 digits + '-' + '.' + \0
	sprintf(percentString, "%.1f", _Percent);

	// This shifts the text over so that the minus sign looks like it just pops up in front without shifting the rest of the text over
	if (_Percent >= 0)
	{
		Dashboard.WriteAt(percentString, _StartX + _TextBoxStartX + _TextBoxTextOffsetX + 6, _StartY + _TextBoxStartY + _TextBoxTextOffsetY, FEHDashboard::BlackColor, 2);
	}
	else
	{
		Dashboard.WriteAt(percentString, _StartX + _TextBoxStartX + _TextBoxTextOffsetX, _StartY + _TextBoxStartY + _TextBoxTextOffsetY, FEHDashboard::BlackColor, 2);
	}
}