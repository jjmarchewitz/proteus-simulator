#ifndef FEHIO_H
#define FEHIO_H

#include <iostream>
#include <string>
#include "Component.h"
#include "tigr.h"

class FEHIO
{
public:
    typedef enum
    {
        P0_0 = 0,
        P0_1,
        P0_2,
        P0_3,
        P0_4,
        P0_5,
        P0_6,
        P0_7,
        P1_0,
        P1_1,
        P1_2,
        P1_3,
        P1_4,
        P1_5,
        P1_6,
        P1_7,
        P2_0,
        P2_1,
        P2_2,
        P2_3,
        P2_4,
        P2_5,
        P2_6,
        P2_7,
        P3_0,
        P3_1,
        P3_2,
        P3_3,
        P3_4,
        P3_5,
        P3_6,
        P3_7,
        BATTERY_VOLTAGE
    } FEHIOPin;

    typedef enum
    {
        Bank0 = 0,
        Bank1,
        Bank2,
        Bank3
    } FEHIOPort;

    typedef enum
    {
        RisingEdge = 0x09,
        FallingEdge = 0x0A,
        EitherEdge = 0x0B
    } FEHIOInterruptTrigger;

    // This is a struct for a button on one of the components listed elsewhere in this class
    // (i.e.) DigitalInputPin, AnalogEncoder, etc.)
    struct FEHIOComponentButton
    {
        std::string Text;
        bool CurrentlyPressed;
        bool PreviouslyPressed;
        int StartX;
        int StartY;
        int Width;
        int Height;
        int TextOffsetX;
        int TextOffsetY;
        int FontSize;
    };

    static bool InitializedPins[37];
};

class DigitalInputPin : Component
{
public:
    DigitalInputPin(FEHIO::FEHIOPin pin);
    bool Value();
    void Draw() override;

    friend class ButtonBoard;

private:
    FEHIO::FEHIOPin _FullPinID;
    bool _Value;

    DigitalInputPin();
    void Initialize(FEHIO::FEHIOPin pin);
    int _Bank, _PinNumInBank;

    // Drawing variables
    int _StartX;
    int _StartY;
    int _CurrentlyPressed;
    int _PreviouslyPressed;
    int _Width;
    int _Height;
    int _ButtonStartX;
    int _ButtonStartY;
    int _ButtonWidth;
    int _ButtonHeight;
    static const int _BorderSize = 2;
};

class DigitalEncoder : Component
{
public:
    DigitalEncoder(FEHIO::FEHIOPin pin, FEHIO::FEHIOInterruptTrigger trigger);
    DigitalEncoder(FEHIO::FEHIOPin pin);

    int Counts();
    void ResetCounts();
    void Draw() override;

private:
    void _UpdateCountFromPressedButtons();

    typedef enum
    {
        PlusTen = 0,
        PlusHundred,
        PlusThousand,
        MinusTen,
        MinusHundred,
        MinusThousand,
        Reset
    } _ButtonIDs;
    static const int _NumOfButtons = 7;

    FEHIO::FEHIOPin _FullPinID;
    int _Bank;
    int _PinNumInBank;
    int _Count;
    char _CountString[80];

    int _StartX;
    int _StartY;
    int _Width;
    int _Height;
    static const int _BorderSize = 2;

    FEHIO::FEHIOComponentButton _Buttons[_NumOfButtons];

    DigitalEncoder();
    void Initialize(FEHIO::FEHIOPin pin);
};

class AnalogInputPin : Component
{
private:
    void _UpdateValueFromPressedButtons();

    typedef enum
    {
        PlusHundredth = 0,
        PlusTenth,
        PlusOne,
        MinusHundredth,
        MinusTenth,
        MinusOne,
        Reset
    } _ButtonIDs;
    static const int _NumOfButtons = 7;

    FEHIO::FEHIOPin _FullPinID;
    int _Bank;
    int _PinNumInBank;
    float _Value;
    char _ValueString[80];

    int _StartX;
    int _StartY;
    int _Width;
    int _Height;
    static const int _BorderSize = 2;

    FEHIO::FEHIOComponentButton _Buttons[_NumOfButtons];

public:
    AnalogInputPin(FEHIO::FEHIOPin);

    float Value();
    void Draw() override;
};

class DigitalOutputPin : Component
{
private:
    FEHIO::FEHIOPin _FullPinID;
    bool _Value;

    // Drawing variables
    int _StartX;
    int _StartY;
    int _Width;
    int _Height;
    int _Bank;
    int _PinNumInBank;
    int _BoxStartX;
    int _BoxStartY;
    int _BoxWidth;
    int _BoxHeight;
    static const int _BorderSize = 2;

public:
    DigitalOutputPin(FEHIO::FEHIOPin);

    void Write(bool);
    bool Status();
    void Toggle();
    void Draw() override;
};

// This class does not inherit from Component because it is deprecated-
// we do not need to implement it as of right now
class ButtonBoard
{
public:
    ButtonBoard(FEHIO::FEHIOPort bank);

    bool LeftPressed();
    bool LeftReleased();
    bool MiddlePressed();
    bool MiddleReleased();
    bool RightPressed();
    bool RightReleased();

private:
    DigitalInputPin _left;
    DigitalInputPin _middle;
    DigitalInputPin _right;
};

class AnalogEncoder : Component
{
private:
    void _UpdateCountFromPressedButtons();

    typedef enum
    {
        PlusTen = 0,
        PlusHundred,
        PlusThousand,
        MinusTen,
        MinusHundred,
        MinusThousand,
        Reset
    } _ButtonIDs;
    static const int _NumOfButtons = 7;

    typedef enum
    {
        LOW_STATE,
        HIGH_STATE
    } EncoderState;

    FEHIO::FEHIOPin _FullPinID;
    int _Bank;
    int _PinNumInBank;

    int _StartX;
    int _StartY;
    int _Width;
    int _Height;

    static const int _BorderSize = 2;

    FEHIO::FEHIOComponentButton _Buttons[_NumOfButtons];

    static void SetCounterInit(unsigned int);

    int _Count;
    char _CountString[80];
    int _LowThreshold;
    int _HighThreshold;

public:
    static void SetRate(unsigned int rateHz);
    AnalogEncoder(FEHIO::FEHIOPin);
    ~AnalogEncoder();

    int Counts();
    void ResetCounts();
    void Draw() override;

    void SetThresholds(float low, float high);
};

#endif // FEHIO_H
