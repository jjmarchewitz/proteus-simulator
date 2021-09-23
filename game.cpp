#include "game.hpp"

int main()
{
    // Tigr *screen = tigrWindow(320, 240, "Test", TIGR_AUTO);
    // while (!tigrClosed(screen))
    // {
    //     tigrClear(screen, tigrRGB(0, 0, 0));
    //     tigrPrint(screen, tfont, 10, 10, tigrRGB(0xff, 0xff, 0xff), "Hello world!");
    //     tigrUpdate(screen);
    // }
    // tigrFree(screen);

    while (1)
    {
        LCD.Clear(FEHLCD::Red);
    }

    return 0;
}