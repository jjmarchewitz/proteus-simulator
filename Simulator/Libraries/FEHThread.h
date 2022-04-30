/*
This file provides a function to create a new thread out of the ProteusMain funcion.
XCode uses std::thread while mingw32's g++ does not (it uses windows.h) so the implementation
details are different between Windows and Mac.  The preprocessor directives in this file and FEHThread.cpp
ensure that both operating systems can build this code.
*/

#ifndef FEHTHREADHEADER
#define FEHTHREADHEADER

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) // For windows:
#include <Windows.h>
#define MUTEX HANDLE

#else // For MacOS
#include <thread>
#include <mutex>
#define MUTEX std::mutex

#endif

void ThreadProteus(); // Function that is called by main (defined in FEHThread.cpp)

class FEHMutex // Used to make component code thread safe.  Different implementations are needed for Windows and Mac
{
public:
    FEHMutex();
    void lock(); // Waits until ownership of the mutex is given up and then takes ownership of it
    void unlock(); // Releases ownership of the mutex
private:
    MUTEX mtx;
};

#endif // FEHTHREADHEADER