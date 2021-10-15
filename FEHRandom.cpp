#include "FEHRandom.h"
#include "FEHLCD.h"

FEHRandom Random;

void FEHRandom::Initialize()
{
}

void FEHRandom::Seed()
{
}

int FEHRandom::RandInt()
{
	return rand(); // steal rand from stdlib.h
}