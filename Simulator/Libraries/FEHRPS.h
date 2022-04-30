#ifndef FEHRPS_H
#define FEHRPS_H

class FEHRPS
{
public:
    void InitializeTouchMenu();

    unsigned char CurrentCourse();

    char CurrentRegionLetter();

    int Time();

    float X();

    float Y();

    float Heading();

    int GetIceCream();

    // returns the number of the current course { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }
    int CurrentRegion();
    // Creates a menu to allow you to pick the correct region
    // Assumes ButtonBoard is plugged into Bank3
    // Right button increments region
    // Left button decrements region
    // Middle button selects region
    void InitializeMenu(); //Deprecated because uses button board
    // Manually pick and configure a region
    // int region => { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }
    // char region => { a, b, c, d, e, f, g, h, i, j, k, l } || { A, B, C, D, E, F, G, H, I, J, K, L }
    void Initialize(int region);
    void Initialize(char region);

    FEHRPS();

private:
    int _region;
};

extern FEHRPS RPS;

#endif // FEHRPS_H