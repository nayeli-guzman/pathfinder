
#include "screen.h"

Screen* Screen::instancia = nullptr;

int main()
{
    //srand(time(nullptr));

    Screen* screen = Screen::getInstance();
    screen->execute();

}
