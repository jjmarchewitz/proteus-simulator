#ifndef FEHMOTOR_H
#define FEHMOTOR_H

#include "Component.h"
#include "FEHDashboard.h"
#include "FEHLCD.h"
#include "tigr.h"

class FEHMotor : Component
{
public:
	typedef enum
	{
		Motor0 = 0,
		Motor1,
		Motor2,
		Motor3
	} FEHMotorPort;

	FEHMotor(FEHMotorPort, float);

	void SetPercent(float percent);
	void Stop();

	void Draw() override;

private:
	unsigned int _Port;
	float _Percent;
	static bool InitializedMotors[4];

	TPixel _Color;

	int _StartX;
	int _StartY;
	int _Width;
	int _Height;

	int _BarStartX;
	int _BarStartY;
	int _BarWidth;
	int _BarMaxHeight;

	int _TextBoxStartX;
	int _TextBoxStartY;
	int _TextBoxWidth;
	int _TextBoxHeight;
	int _TextBoxTextOffsetX;
	int _TextBoxTextOffsetY;

	static const int _BorderSize = 2;
	static const int _TextBoxBorderSize = 2;
};

#endif //FEHMOTOR_H