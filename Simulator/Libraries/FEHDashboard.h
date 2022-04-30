#ifndef FEHDASHBOARD_H
#define FEHDASHBOARD_H

#include <string>
#include <vector>
#include "Component.h"
#include "FEHLCD.h"
#include "FEHThread.h"
#include "LCDColors.h"
#include "tigr.h"

// This line is needed because Dashboard may be defined before Component
class Component;

class FEHDashboard
{
public:
    friend class Component;
    FEHDashboard();

    void AddComponent(Component *);
    void GetCellDimensions(int *, int *);
    bool MouseWasJustClicked();

    inline Tigr *Screen() { return _Screen; };
    void Update();

    void WriteAt(const char *, int, int, TPixel, int);
    void WriteAt(int, int, int, TPixel, int);
    void WriteAt(float, int, int, TPixel, int);
    void WriteAt(double, int, int, TPixel, int);
    void WriteAt(bool, int, int, TPixel, int);
    void WriteCharAt(int, int, char, TPixel, int);
    inline void Init() { _Initialize(); }

    static const TPixel BoolTrueColor, BoolFalseColor, BlackColor, WhiteColor, ScarletColor, GreyColor, BackgroundColor, RedColor, GreenColor, BlueColor;

private:
    void _Initialize();
    void _ClearDash();
    void _DrawUIBackground();
    Tigr *_Screen;
    bool _ClickState[2];

    std::vector<Component *> _Components;
};

extern FEHDashboard Dashboard;

#endif // DASHBOARD_H
