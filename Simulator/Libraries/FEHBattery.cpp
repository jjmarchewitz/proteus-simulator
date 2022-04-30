#include "FEHBattery.h"

FEHBattery Battery(FEHIO::BATTERY_VOLTAGE);

FEHBattery::FEHBattery(FEHIO::FEHIOPin pin_) : AnalogInputPin(pin_) {}
float FEHBattery::Voltage() {}
