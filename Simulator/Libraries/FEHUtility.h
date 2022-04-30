#ifndef FEHUTILITY_H
#define FEHUTILITY_H

#include <sys/time.h>
#include "FEHLCD.h"

void Sleep(int msec);
void Sleep(float sec);
void Sleep(double sec);

double TimeNow();
unsigned int TimeNowSec();
unsigned long TimeNowMSec();
void ResetTime();

// TODO: Add this to the documentation website
inline bool RunningOnProteus() { return false; };
inline bool RunningOnSimulator() { return true; };

#endif // FEHUTILITY_H
