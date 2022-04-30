#include "FEHLCD.h"
#include "FEHDashboard.h"

// Entry point of the code.  This creates a thread out of ProteusMain
int main()
{
    Dashboard.Init();
    ThreadProteus(); // Creates the thread for ProteusMain.  This function also enters the rendering loop for LCD and Dashboard

    return 0;
}