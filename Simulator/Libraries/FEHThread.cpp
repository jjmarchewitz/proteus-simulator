#include <iostream>
#include "FEHThread.h"
#include "FEHDashboard.h"

extern FEHDashboard Dashboard;
extern int ProteusMain();

void RenderLoop() // For both Windows and MacOS, this function will be called by the main thread
{
    while (true)
    {
        // See comment in zero-argument LCD.Update() definition for explanation on why true is passed
        LCD.Update(true);
        Dashboard.Update();
    }
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) // For Windows
DWORD WINAPI runProteus(LPVOID PARAM)
{
    ProteusMain();
}

void ThreadProteus() // Uses the Windows.h
{
    HANDLE t = CreateThread(NULL, 0, runProteus, NULL, 0, NULL); // Creates thread.  Does not need to keep t in scope
    if (t == NULL)
    {
        std::cout << "ERROR: could not create Windows dashboard process";
    }
    RenderLoop();
}

FEHMutex::FEHMutex()
{
    // Create a mutex with no initial owner
    mtx = CreateMutex(NULL, FALSE, NULL);
}

void FEHMutex::lock()
{
    WaitForSingleObject(mtx, INFINITE);
}

void FEHMutex::unlock()
{
    ReleaseMutex(mtx);
}

#else // For Mac
FEHMutex::FEHMutex()
{
}

void ThreadProteus()
{
    std::thread t(ProteusMain);
    RenderLoop();
    t.join(); // This line is needed or else the program will crash on MacOS even though the code should never reach this point.
}

void FEHMutex::lock()
{
    mtx.lock();
}

void FEHMutex::unlock()
{
    mtx.unlock();
}
#endif