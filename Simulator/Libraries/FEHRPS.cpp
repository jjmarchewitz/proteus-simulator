#include "FEHRPS.h"

FEHRPS RPS;

void FEHRPS::InitializeTouchMenu() {}

unsigned char FEHRPS::CurrentCourse() {}

char FEHRPS::CurrentRegionLetter() {}

int FEHRPS::Time() {}

float FEHRPS::X() {}

float FEHRPS::Y() {}

float FEHRPS::Heading() {}

int FEHRPS::GetIceCream() {}

// returns the number of the current course { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }
int FEHRPS::CurrentRegion() {}
// Creates a menu to allow you to pick the correct region
// Assumes ButtonBoard is plugged into Bank3
// Right button increments region
// Left button decrements region
// Middle button selects region
void FEHRPS::InitializeMenu() {} //Deprecated because uses button board
// Manually pick and configure a region
// int region => { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }
// char region => { a, b, c, d, e, f, g, h, i, j, k, l } || { A, B, C, D, E, F, G, H, I, J, K, L }
void FEHRPS::Initialize(int region) {}
void FEHRPS::Initialize(char region) {}

FEHRPS::FEHRPS() {}
