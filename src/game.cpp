#include "game.hpp"

int main() {
    Tigr *screen = tigrWindow(320, 240, "Test", 0);
    while (!tigrClosed(screen)) {
        tigrClear(screen, tigrRGB(0, 0, 0));
        tigrPrint(screen, tfont, 10, 10, tigrRGB(0xff, 0xff, 0xff), "Hello world!");
        tigrUpdate(screen);
    }
    tigrFree(screen);
    return 0;
}