#ifndef COMPONENT_H
#define COMPONENT_H

#include "FEHDashboard.h"
#include "FEHThread.h"

extern FEHMutex dashMutex;

class Component
{
public:
	Component();
	virtual void Draw() = 0;
};

#endif // COMPONENT_H
