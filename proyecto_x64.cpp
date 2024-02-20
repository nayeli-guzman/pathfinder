
#include "screen.h"
#include <ctime>

Screen* Screen::instancia = nullptr;

int main()
{
    srand(time(nullptr));

    cout << "Cargando el laberinto...";

    Screen* screen = Screen::getInstance();
    screen->execute();

}
